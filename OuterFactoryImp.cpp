#include <sstream>
#include "OuterFactoryImp.h"
#include "LogComm.h"
#include "ConfigServer.h"

//
using namespace wbl;

/**
 *
*/
OuterFactoryImp::OuterFactoryImp(): _pFileConf(NULL)
{
    createAllObject();
}

/**
 *
*/
OuterFactoryImp::~OuterFactoryImp()
{
    deleteAllObject();
}

/**
 *
*/
void OuterFactoryImp::deleteAllObject()
{
    if(NULL != _pFileConf)
    {
        //服务配置
        delete _pFileConf;
        _pFileConf = NULL;
    }
}

/**
 *
*/
void OuterFactoryImp::createAllObject()
{
    __TRY__

    //删除
    deleteAllObject();

    //tars代理Factory,访问其他tars接口时使用
    _pProxyFactory = new OuterProxyFactory();
    if((long int)NULL == _pProxyFactory)
    {
        ROLLLOG_ERROR << "create outer proxy factory fail, ptr null." << endl;
        terminate();
    }

    //
    LOG_DEBUG << "init proxy factory succ." << endl;

    //加载配置
    load();

    __CATCH__
    return;
}

void OuterFactoryImp::load()
{
    __TRY__

    //拉取远程配置
    g_app.addConfig(ServerConfig::ServerName + ".conf");

    //本地配置文件
    _pFileConf = new tars::TC_Config();
    if (NULL == _pFileConf)
    {
        ROLLLOG_ERROR << "create config parser fail, ptr null." << endl;
        terminate();
    }

    //
    _pFileConf->parseFile(ServerConfig::BasePath + ServerConfig::ServerName + ".conf");
    LOG_DEBUG << "init config file succ, base path: " << ServerConfig::BasePath + ServerConfig::ServerName + ".conf" << endl;

    //读取所有配置
    readAllConfig();
    __CATCH__
}

// 读取所有配置
void OuterFactoryImp::readAllConfig()
{
    __TRY__

    WriteLocker lock(m_rwlock);

    // 读取数据类型配置
    readDBConfig();
    printDBConfig();

    // 锁的数量
    readLockNum();
    printLockNum();

    __CATCH__
}

// 读取db配置
void OuterFactoryImp::readDBConfig()
{
    dbConf.Domain = (*_pFileConf).get("/Main/db<domain>", "");
    dbConf.Host = (*_pFileConf).get("/Main/db<host>", "");
    dbConf.port = (*_pFileConf).get("/Main/db<port>", "3306");
    dbConf.user = (*_pFileConf).get("/Main/db<user>", "tars");
    dbConf.password = (*_pFileConf).get("/Main/db<password>", "tars2015");
    dbConf.charset = (*_pFileConf).get("/Main/db<charset>", "utf8");
    dbConf.dbname = (*_pFileConf).get("/Main/db<dbname>", "");

    //域名
    if (dbConf.Domain.length() > 0)
    {
        string szHost = getIp(dbConf.Domain);
        if (szHost.length() > 0)
        {
            dbConf.Host = szHost;
            ROLLLOG_DEBUG << "get host by domain, Domain: " << dbConf.Domain << ", szHost: " << szHost << endl;
        }
    }
}

void OuterFactoryImp::printDBConfig()
{
    FDLOG_CONFIG_INFO << "Domain: " << dbConf.Domain
                      << ", Host: " << dbConf.Host
                      << ", port: " <<  dbConf.port
                      << ", user: " << dbConf.user
                      << ", password: " << dbConf.password
                      << ", charset: " << dbConf.charset
                      << ", dbname: " << dbConf.dbname << endl;
}

//读取锁数量配置
void OuterFactoryImp::readLockNum()
{
    lockNum = TC_Common::strto<int>( (*_pFileConf).get("/Main/lock_num<number>", "10"));
}

//打印锁数量配置
void OuterFactoryImp::printLockNum()
{
    FDLOG_CONFIG_INFO << "lock num: " << lockNum << endl;
}

//取锁数量配置
int OuterFactoryImp::getLockNum()
{
    wbl::ReadLocker lock(m_rwlock);
    return lockNum;
}

//拆分字符串成整形
int OuterFactoryImp::splitInt(string szSrc, vector<int> &vecInt)
{
    split_int(szSrc, "[ \t]*\\|[ \t]*", vecInt);
    return 0;
}

//格式化时间
string OuterFactoryImp::timeFormat(const string &sFormat, time_t timeCluster)
{
    //string sFormat("%Y-%m-%d %H:%M:%S");
    if (sFormat.length() == 0)
        return "";

    //time
    time_t t = timeCluster;
    auto pTm = localtime(&t);
    if (pTm == NULL)
        return "";

    //format
    char sTimeString[255] = "\0";
    strftime(sTimeString, sizeof(sTimeString), sFormat.c_str(), pTm);
    return string(sTimeString);
}

//域名解析
void OuterFactoryImp::getIp(char *domain, char *ip)
{
    if (!domain || !ip)
        return;

    if (strlen(domain) == 0)
        return;

    struct hostent host = *gethostbyname(domain);
    for (int i = 0; host.h_addr_list[i]; i++)
    {
        strcpy(ip, inet_ntoa(*(struct in_addr *)host.h_addr_list[i]));
        break;
    }
}

//域名解析
string OuterFactoryImp::getIp(const string &domain)
{
    if(domain.length() == 0)
    {
        return "";
    }

    struct hostent host = *gethostbyname(domain.c_str());
    for (int i = 0; host.h_addr_list[i]; i++)
    {
        string ip = inet_ntoa(*(struct in_addr *)host.h_addr_list[i]);
        return ip;
    }

    return "";
}

//比赛时间解析
#define ONE_DAY_TIME (24*60*60)
#define ZONE_TIME_OFFSET (8*60*60)
long OuterFactoryImp::getFormatTime(int type, long start_time, int week_time)
{
    ROLLLOG_DEBUG << "type:" << type << ", start_time:" << start_time << ", week_time:" << week_time << endl;
    /*    week_time = week_time == 0 ? 7 : week_time;*/
    time_t now = time(0);
    if(type == 0 || type == 1)
    {
        return start_time;
    }
    else if(type == 2)// every week
    {
        tm *n_tm = localtime(&now);
        n_tm->tm_hour = 0;
        n_tm->tm_min = 0;
        n_tm->tm_sec = 0;

        long today_use_time = (TNOW + ZONE_TIME_OFFSET) % ONE_DAY_TIME;
        if(n_tm->tm_wday > week_time || (n_tm->tm_wday == week_time && today_use_time > start_time)) //next week
        {
            n_tm->tm_mday += (7 - (n_tm->tm_wday - week_time ));
        }
        else
        {
            n_tm->tm_mday += (week_time - n_tm->tm_wday);
        }
        ROLLLOG_DEBUG << "time1:" << mktime(n_tm) << ", time:" << start_time + mktime(n_tm) << endl;
        return mktime(n_tm) + start_time;
    }
    else if(type == 3)//every month
    {
        tm *month_tm = localtime(&now);

        //本月最后一天
        bool b_last_month = month_tm->tm_mon == 11;
        month_tm->tm_year = b_last_month ? month_tm->tm_year + 1 : month_tm->tm_year;
        month_tm->tm_mon = b_last_month ? 0 : month_tm->tm_mon + 1;
        month_tm->tm_hour = 0;
        month_tm->tm_min = 0;
        month_tm->tm_sec = 0;
        month_tm->tm_mday = 0;
        time_t  next_mouth_time = mktime(month_tm);
        tm *next_month_tm = localtime(&next_mouth_time);

        //month last day  week
        if(week_time > next_month_tm->tm_wday) // back n days
        {
            next_month_tm->tm_mday -= (7 - week_time + next_month_tm->tm_wday);
        }
        else
        {
            next_month_tm->tm_mday -= (next_month_tm->tm_wday - week_time);
        }
        ROLLLOG_DEBUG << "time1:" << mktime(next_month_tm) << ", time:" << start_time + mktime(next_month_tm) << endl;
        return mktime(next_month_tm) + start_time;
    }
    return 0;
}
