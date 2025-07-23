#ifndef _OUTER_FACTORY_IMP_H_
#define _OUTER_FACTORY_IMP_H_

#include <string>
#include "globe.h"
#include "OuterFactory.h"
#include "ConfigDefine.h"

//wbl
#include <wbl/regex_util.h>

//
using namespace tars;

/**
 * 外部工具接口对象工厂
 */
class OuterFactoryImp : public OuterFactory
{
public:
    /**
     *
    */
    OuterFactoryImp();

    /**
     *
    */
    virtual ~OuterFactoryImp();

private:
    /**
     *
    */
    void createAllObject();

    /**
     *
    */
    void deleteAllObject();

public:
    //加载配置
    void load();

public:
    // 获取代理

public:
    //框架中用到的outer接口(不能修改):
    const OuterProxyFactoryPtr &getProxyFactory() const
    {
        return _pProxyFactory;
    }

    const tars::TC_Config &getConfig() const
    {
        return *_pFileConf;
    }

public:
    // 读取所有配置
    void readAllConfig();

    // 读取db配置
    void readDBConfig();
    //
    void printDBConfig();
    //
    const config::DBConf &getDBConfig()
    {
        return dbConf;
    }

    long getFormatTime(int type, long start_time, int week_time);

public:
    //读取锁数量配置
    void readLockNum();
    //打印锁数量配置
    void printLockNum();
    //取锁数量配置
    int getLockNum();

private:
    //拆分字符串成整形
    int splitInt(string szSrc, vector<int> &vecInt);
    //格式化时间
    string timeFormat(const string &sFormat, time_t timeCluster);

private:
    //域名解析
    void getIp(char *domain, char *ip);
    //域名解析
    string getIp(const string &domain);

private:
    //框架用到的共享对象(不能修改):
    tars::TC_Config *_pFileConf;  //服务配置

private:
    OuterProxyFactoryPtr _pProxyFactory;

private:
    //业务自有的共享对象:
    //...

private:
    wbl::ReadWriteLocker m_rwlock; //读写锁，防止脏读

private:
    config::DBConf dbConf; //数据源配置

private:
    int lockNum;  //互斥锁数量
};

//ptr
typedef TC_AutoPtr<OuterFactoryImp> OuterFactoryImpPtr;

#endif


