#include <sstream>
#include <random>
#include "OuterFactoryImp.h"
#include "LogComm.h"
#include "SocialServer.h"


/**
 *
*/
OuterFactoryImp::OuterFactoryImp(): _pFileConf(NULL)
{
    createAllObject();
}

OuterFactoryImp::~OuterFactoryImp()
{
    deleteAllObject();
}

void OuterFactoryImp::deleteAllObject()
{
    if (_pFileConf)
    {
        delete _pFileConf;
        _pFileConf = NULL;
    }
}

void OuterFactoryImp::createAllObject()
{
    try
    {
        deleteAllObject();

        //本地配置文件
        _pFileConf = new tars::TC_Config();

        //tars代理Factory,访问其他tars接口时使用
        _pProxyFactory = new OuterProxyFactory();
        LOG_DEBUG << "init proxy factory succ." << endl;

        //加载配置
        load();
    }
    catch (TC_Exception &ex)
    {
        LOG->error() << ex.what() << endl;
        throw;
    }
    catch (exception &e)
    {
        LOG->error() << e.what() << endl;
        throw;
    }
    catch (...)
    {
        LOG->error() << "unknown exception." << endl;
        throw;
    }

    return;
}

//读取所有配置
void OuterFactoryImp::load()
{
    __TRY__

    //拉取远程配置
    g_app.addConfig(ServerConfig::ServerName + ".conf");

    WriteLocker lock(m_rwlock);

    //
    _pFileConf->parseFile(ServerConfig::BasePath + ServerConfig::ServerName + ".conf");
    LOG_DEBUG << "init config file succ:" << ServerConfig::BasePath + ServerConfig::ServerName + ".conf" << endl;

    //代理配置
    readPrxConfig();
    printPrxConfig();

    // //最大好友数量
    // readMaxFriendsCount();
    // printMaxFriendsCount();

    // 创建俱乐部费用
    readCreateClubCost();

    //邮件配置
    readMailConfig();
    printMailConfig();

    //加载通用配置
    readGeneralConfigResp();
    printGeneralConfigResp();

    // 加载俱乐部等级配置
    loadClubLevelConfig();
    printClubLevelConfig();

    // 加载俱乐部商户等级配置
    loadClubLevelCoinConfig();
    printClubLevelCoinConfig();

    // 加载俱乐部基金配置
    loadClubCoinConfig();
    printClubCoinConfig();

    // 加载VIP信息配置
    loadSysVipConfig();
    printSysVipConfig();

    // 加载CONST信息配置
    loadSysConstConfig();
    printSysConstConfig();

    // 加载联盟配置
    loadUnionLevelConfig();
    printUnionLevelConfig();

    __CATCH__
}

//代理配置
void OuterFactoryImp::readPrxConfig()
{
    _ConfigServantObj = (*_pFileConf).get("/Main/Interface/ConfigServer<ProxyObj>", "");
    _DBAgentServantObj = (*_pFileConf).get("/Main/Interface/DBAgentServer<ProxyObj>", "");
    _PushServantObj = (*_pFileConf).get("/Main/Interface/PushServer<ProxyObj>", "");
    _HallServantObj = (*_pFileConf).get("/Main/Interface/HallServer<ProxyObj>", "");
    _GlobalServantObj = (*_pFileConf).get("/Main/Interface/GlobalServer<ProxyObj>", "");
    _Log2DBServantObj = (*_pFileConf).get("/Main/Interface/Log2DBServer<ProxyObj>", "");
    _OrderServantObj = (*_pFileConf).get("/Main/Interface/OrderServer<ProxyObj>", "");
}

//
void OuterFactoryImp::printPrxConfig()
{
    ROLLLOG_DEBUG << "_ConfigServantObj ProxyObj:" << _ConfigServantObj << endl;
    ROLLLOG_DEBUG << "_DBAgentServantObj ProxyObj:" << _DBAgentServantObj << endl;
    ROLLLOG_DEBUG << "_PushServantObj ProxyObj:" << _PushServantObj << endl;
    ROLLLOG_DEBUG << "_HallServantObj ProxyObj:" << _HallServantObj << endl;
    ROLLLOG_DEBUG << "_GlobalServantObj ProxyObj:" << _GlobalServantObj << endl;
    ROLLLOG_DEBUG << "_Log2DBServantObj ProxyObj:" << _Log2DBServantObj << endl;
    ROLLLOG_DEBUG << "_OrderServantObj ProxyObj:" << _OrderServantObj << endl;
}

//邮件配置
void OuterFactoryImp::readMailConfig()
{
    //邮件模板
    mapMailConfig.clear();
    auto vecDomainKey = (*_pFileConf).getDomainVector("/Main/mail");
    for (auto &domain : vecDomainKey)
    {
        MailConfig st;
        string subDomain = "/Main/mail/" + domain;
        st.sContent = (*_pFileConf).get(subDomain + "<content>");
        st.sTitle = (*_pFileConf).get(subDomain + "<title>");
        mapMailConfig.insert(std::make_pair(TC_Common::strto<int>(domain), st));
    }
}

//输出邮件配置
void OuterFactoryImp::printMailConfig()
{
    std::string str;
    for (auto &it : mapMailConfig)
    {
        str.append("type:").append(I2S(it.first)).append("\t\n");
        str.append("title:").append(it.second.sTitle).append("\t\n");
        str.append("content:").append(it.second.sContent).append("\t\n");
    }
    ROLLLOG_DEBUG << "mapMailConfig :" << str << endl;
}

//加载俱乐部等级配置
void OuterFactoryImp::loadClubLevelConfig()
{
    ROLLLOG_DEBUG << "loadClubLevelConfig :" << endl;
    config::ClubLevelCfgResp resp;
    int iRet = getConfigServantPrx()->getClubLevelCfg(resp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "LoadClubLevelConfig ERROR. iRet:" << iRet << endl;
        return;
    }

    m_mapClubLevelCfg.clear();
    m_mapClubLevelCfg = resp.data;
}

//输出俱乐部等级配置
void OuterFactoryImp::printClubLevelConfig()
{
    std::string str;
    for (auto &it : m_mapClubLevelCfg)
    {
        str.append("level:").append(I2S(it.first)).append("\t\n");
        str.append("peopleMax:").append(I2S(it.second.peopleMax)).append("\t\n");
        str.append("adminMax:").append(I2S(it.second.adminMax)).append("\t\n");
        str.append("activityOne:").append(L2S(it.second.activityOne)).append("\t\n");
        str.append("activityTwo:").append(L2S(it.second.activityTwo)).append("\t\n");
        str.append("upFast:").append(L2S(it.second.upFast)).append("\t\n");
        str.append("levelDay:").append(L2S(it.second.levelDay)).append("\t\n");
    }
    ROLLLOG_DEBUG << "m_mapClubLevelCfg :" << endl << str << endl;
}

// 获取俱乐部等级配置
int OuterFactoryImp::getClubLevelConfig(int iId, config::ClubLevelConfig &cfg)
{
    auto it = m_mapClubLevelCfg.find(iId);
    if (it != m_mapClubLevelCfg.end())
    {
        cfg = it->second;
        return 0;
    }

    return -1;
}

// 获取升级俱乐部等级配置
int OuterFactoryImp::getUpClubLevelConfig(long exp, config::ClubLevelConfig &cfg)
{
    for (auto item : m_mapClubLevelCfg)
    {
        if (item.second.activityOne <= exp)
        {
            cfg = item.second;
        }
        else
        {
            return 0;
        }
    }

    return -1;
}

// 获取降级俱乐部等级配置
int OuterFactoryImp::getDownClubLevelConfig(long exp, config::ClubLevelConfig &cfg)
{
    for (auto item : m_mapClubLevelCfg)
    {
        if (item.second.activityTwo <= exp)
        {
            cfg = item.second;
        }
        else
        {
            return 0;
        }
    }

    return -1;
}

//加载俱乐部商户等级配置
void OuterFactoryImp::loadClubLevelCoinConfig()
{
    ROLLLOG_DEBUG << "loadClubLevelCoinConfig :" << endl;
    config::ClubLevelCoinCfgResp resp;
    int iRet = getConfigServantPrx()->getClubLevelCoinCfg(resp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "loadClubLevelCoinConfig ERROR. iRet:" << iRet << endl;
        return;
    }

    m_mapClubLevelCoinCfg.clear();
    m_mapClubLevelCoinCfg = resp.data;

    m_club_max_merchant_level = 0;
    for (auto item : m_mapClubLevelCoinCfg)
    {
        if (item.second.level > m_club_max_merchant_level)
        {
            m_club_max_merchant_level = item.first;
        }
    }
}

//输出俱乐部商户等级配置
void OuterFactoryImp::printClubLevelCoinConfig()
{
    std::string str;
    for (auto &it : m_mapClubLevelCoinCfg)
    {
        str.append("level:").append(I2S(it.second.level)).append("\t\n");
        str.append("maxFee:").append(I2S(it.second.maxFee)).append("\t\n");
        str.append("minCoin:").append(I2S(it.second.minCoin)).append("\t\n");
        str.append("totalCoin:").append(L2S(it.second.totalCoin)).append("\t\n");
        str.append("warnFee:").append(I2S(it.second.warnFee)).append("\t\n");
        str.append("warnCoin:").append(L2S(it.second.warnCoin)).append("\t\n");

    }
    ROLLLOG_DEBUG << "m_mapClubLevelCoinCfg :" << endl << str << endl;
}

// 获取俱乐部商户等级配置
int OuterFactoryImp::getClubLevelCoinConfig(int iLevel, config::ClubLevelCoinConfig &cfg)
{
     auto it = m_mapClubLevelCoinCfg.find(iLevel);
    if (it != m_mapClubLevelCoinCfg.end())
    {
        cfg = it->second;
        return 0;
    }

    return -1;
}

int OuterFactoryImp::getClubLevelCoinMaxLevel()
{
    return m_club_max_merchant_level;
}

//加载俱乐部基金配置
void OuterFactoryImp::loadClubCoinConfig()
{
    ROLLLOG_DEBUG << "loadClubCoinConfig :" << endl;
    config::ClubCoinCfgResp resp;
    int iRet = getConfigServantPrx()->getClubCoinCfg(resp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "LoadClubCoinConfig ERROR. iRet:" << iRet << endl;
        return;
    }

    m_mapClubCoinCfg.clear();
    m_mapClubCoinCfg = resp.data;
}

//输出俱乐部基金配置
void OuterFactoryImp::printClubCoinConfig()
{
    std::string str;
    for (auto &it : m_mapClubCoinCfg)
    {
        str.append("id:").append(I2S(it.first)).append("\t\n");
        str.append("diamond:").append(I2S(it.second.diamond)).append("\t\n");
        str.append("number:").append(I2S(it.second.number)).append("\t\n");
        str.append("extra:").append(L2S(it.second.extra)).append("\t\n");
    }
    ROLLLOG_DEBUG << "m_mapClubCoinCfg :" << endl << str << endl;
}

// 获取俱乐部基金配置
int OuterFactoryImp::getClubCoinConfig(int iId, config::ClubCoinConfig &cfg)
{
    auto it = m_mapClubCoinCfg.find(iId);
    if (it != m_mapClubCoinCfg.end())
    {
        cfg = it->second;
        return 0;
    }

    return -1;
}

//加载VIP信息配置
void OuterFactoryImp::loadSysVipConfig()
{
    ROLLLOG_DEBUG << "loadSysVipConfig :" << endl;
    config::SysVipCfgResp resp;
    int iRet = getConfigServantPrx()->getSysVipCfg(resp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "loadSysVipConfig ERROR. iRet:" << iRet << endl;
        return;
    }

    m_mapSysVipCfg.clear();
    m_mapSysVipCfg = resp.data;
}

//输出VIP信息配置
void OuterFactoryImp::printSysVipConfig()
{
    std::string str;
    for (auto &it : m_mapSysVipCfg)
    {
        str.append("level:").append(I2S(it.first)).append("\t\n");
        str.append("permissions:[ ");
        for(auto item : it.second.permissions)
        {
            str.append(I2S(item));
            str.append(" ");
        }
        str.append("]\t\n");
        str.append("permissionNums:[ ");
        for(auto item : it.second.permissionNums)
        {
            str.append(I2S(item));
            str.append(" ");
        }
        str.append("]\t\n");
        str.append("describe:").append(it.second.describe).append("\t\n");
        str.append("price:").append(I2S(it.second.price)).append("\t\n");
        str.append("exp:").append(I2S(it.second.exp)).append("\t\n");
    }
    ROLLLOG_DEBUG << "m_mapSysVipCfg :" << endl << str << endl;
}

// 获取VIP信息配置
int OuterFactoryImp::getSysVipConfig(int iId, config::SysVipConfig &cfg)
{
    auto it = m_mapSysVipCfg.find(iId);
    if (it != m_mapSysVipCfg.end())
    {
        cfg = it->second;
        return 0;
    }

    return -1;
}

//加载CONST信息配置
void OuterFactoryImp::loadSysConstConfig()
{
    ROLLLOG_DEBUG << "loadSysConstConfig :" << endl;
    config::SysConstCfgResp resp;
    int iRet = getConfigServantPrx()->getSysConstCfg(resp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "loadSysConstConfig ERROR. iRet:" << iRet << endl;
        return;
    }

    m_mapSysConstCfg.clear();
    m_mapSysConstCfg = resp.data;
}

//输出CONST信息配置
void OuterFactoryImp::printSysConstConfig()
{
    std::string str;
    for (auto &it : m_mapSysConstCfg)
    {
        str.append("name:").append(it.first).append("\t\n");
        str.append("value:").append(I2S(it.second.value)).append("\t\n");
    }
    ROLLLOG_DEBUG << "m_mapSysConstCfg :" << endl << str << endl;
}

// 获取CONST信息配置
int OuterFactoryImp::getSysConstConfig(string sName, config::SysConstConfig &cfg)
{
    auto it = m_mapSysConstCfg.find(sName);
    if (it != m_mapSysConstCfg.end())
    {
        cfg = it->second;
        return 0;
    }

    return -1;
}

//创建俱乐部费用
void OuterFactoryImp::readCreateClubCost()
{
    m_create_club_cost = TC_Common::strto<long>((*_pFileConf).get("/Main<CreateClubCost>", "10000"));
}

long OuterFactoryImp::getCreateClubCost()
{
    return m_create_club_cost;
}

// 加载联盟等级配置
void OuterFactoryImp::loadUnionLevelConfig()
{
    ROLLLOG_DEBUG << "loadUnionLevelConfig :" << endl;
    config::UnionLevelCfgResp resp;
    int iRet = getConfigServantPrx()->getUnionLevelCfg(resp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "loadUnionLevelConfig ERROR. iRet:" << iRet << endl;
        return;
    }

    m_mapUnionLevelCfg.clear();
    m_mapUnionLevelCfg = resp.data;
}

// 输出联盟等级配置
void OuterFactoryImp::printUnionLevelConfig()
{
    std::string str;
    for (auto &it : m_mapUnionLevelCfg)
    {
        str.append("level_id:").append(I2S(it.second.levelId)).append("\t\n");
        str.append("union_mem:").append(L2S(it.second.unionMem)).append("\t\n");
        str.append("up_level_cost:").append(L2S(it.second.upLevelCost)).append("\t\n");
    }

    ROLLLOG_DEBUG << "m_mapUnionLevelCfg :" << endl << str << endl;
}

// 获取联盟等级配置
int OuterFactoryImp::getUnionLevelConfig(int iLevel, config::UnionLevelConfig &cfg)
{
    auto it = m_mapUnionLevelCfg.find(iLevel);
    if (it != m_mapUnionLevelCfg.end())
    {
        cfg = it->second;
        return 0;
    }

    return -1;
}

//最大好友数量
// void OuterFactoryImp::readMaxFriendsCount()
// {
//     maxFriendsCount = TC_Common::strto<int>((*_pFileConf).get("/Main<MaxFriendsCount>", "0"));
// }

// void OuterFactoryImp::printMaxFriendsCount()
// {
//     ROLLLOG_DEBUG << "maxFriendsCount : " << maxFriendsCount << endl;
// }

//最大好友数量
int OuterFactoryImp::getMaxFriendsCount()
{
    int type = config::E_GENERAL_TYPE_FRIENDS_MAXNUM;//通用配置-添加好友数量上限
    auto iter = listGeneralConfigResp.data.find(type);
    if ((iter == listGeneralConfigResp.data.end()) || ((int)iter->second.size() != 1))
    {
        ROLLLOG_ERROR << "getMaxFriendsCount failed, type: " << type << ", size: " << iter->second.size() << endl;
        return 0;
    }
    auto itCfg = iter->second.begin();
    if (itCfg->second.value < 0)
    {
        ROLLLOG_ERROR << "listGeneralConfigResp value error, type: " << type << ", value: " << itCfg->second.value << endl;
        return 0;
    }

    ROLLLOG_DEBUG << "listGeneralConfigResp MaxFriendsCount:" << itCfg->second.value << ", type:" << type << endl;
    return itCfg->second.value;
}

//加载通用配置
void OuterFactoryImp::readGeneralConfigResp()
{
    getConfigServantPrx()->ListGeneralConfig(listGeneralConfigResp);
}

void OuterFactoryImp::printGeneralConfigResp()
{
    ROLLLOG_DEBUG << "listGeneralConfigResp: " << printTars(listGeneralConfigResp) << endl;
}

//格式化时间
string OuterFactoryImp::GetTLogTimeFormat()
{
    string sFormat("%Y-%m-%d %H:%M:%S");
    time_t t = time(NULL);
    struct tm *pTm = localtime(&t);
    if (pTm == NULL)
    {
        return "";
    }

    char sTimeString[255] = "\0";
    strftime(sTimeString, sizeof(sTimeString), sFormat.c_str(), pTm);
    return string(sTimeString);
}

//格式化自定义时间
string OuterFactoryImp::GetCustomTimeFormat(int time)
{
    string sFormat("%Y-%m-%d %H:%M:%S");
    time_t t = time_t(time);
    auto pTm = localtime(&t);
    if (pTm == NULL)
    {
        return "";
    }

    char sTimeString[255] = "\0";
    strftime(sTimeString, sizeof(sTimeString), sFormat.c_str(), pTm);
    return string(sTimeString);
}

//获取自定义秒数
int OuterFactoryImp::GetCustomTimeTick(const string &str)
{
    if(str.empty())
    {
        return 0;
    }

    //
    struct tm tm_time;

    string sFormat("%Y-%m-%d %H:%M:%S");

    strptime(str.c_str(), sFormat.c_str(), &tm_time);

    return mktime(&tm_time);
}

//格式化自定义年月日
int OuterFactoryImp::GetCustomDateFormat(int time)
{
    string sFormat("%Y%m%d");
    time_t t = time_t(time);
    struct tm *pTm = localtime(&t);
    if (pTm == NULL)
    {
        return 0;
    }

    char sDateString[10] = "\0";
    strftime(sDateString, sizeof(sDateString), sFormat.c_str(), pTm);

    return S2I(sDateString);
}

//推送消息
void OuterFactoryImp::asyncRequest2Push(const long uid, const push::PushMsgReq &msg)
{
    getPushServantPrx(uid)->async_pushMsg(NULL, msg);
}

//邮件通知
int OuterFactoryImp::asyncClubMailNotify(const long lUid, const int type, const int mailType, const long clubId, std::vector<string> vParam)
{
    mail::TSendMailReq tSendMailReq;
    tSendMailReq.uid = lUid;
    mail::TMailData &mailData = tSendMailReq.data;
    mailData.type = type; //mail::E_MAIL_TYPE_USER;//个人邮件
    mailData.extra = I2S(clubId);

    auto it = mapMailConfig.find(mailType);
    if(it != mapMailConfig.end())
    {
        mailData.title = it->second.sTitle;
        mailData.content = it->second.sContent;
    }
   
    for(unsigned int i = 0; i < vParam.size(); i++)
    {
        size_t pos = mailData.content.find("&");
        if( pos != std::string::npos)
        {
            mailData.content.replace(pos, 1, vParam[i]);
        }
    }

    mail::TSendMailResp tSendMailResp;
    int iRet = getHallServantPrx(lUid)->sendMailToUserFromSystem(tSendMailReq, tSendMailResp);
    if(iRet != 0)
    {
        LOG_ERROR<< "send mail err. lUid: "<< lUid<< endl;
        return iRet;
    }
    return 0;
}

//修改用户帐号接口
int OuterFactoryImp::async2ModifyUWealth(const userinfo::ModifyUserWealthReq &req)
{
    auto pHallServant = getHallServantPrx(req.uid);
    if (!pHallServant)
    {
        LOG_ERROR << "pHallServant is nullptr" << endl;
        return -1;
    }

    pHallServant->async_modifyUserWealth(NULL, req);
    LOG_DEBUG << "uid: " << req.uid << ", " << printTars(req)  << "}\n <<<< End \n\n\n\n" << endl;
    return 0;
}

//修改用户帐号接口(tg)
int OuterFactoryImp::async2ModifyUWalletBalance(const order::ModifyWalletBalanceReq &req)
{
    auto pOrderServant = getOrderServantPrx(req.uid);
    if (!pOrderServant)
    {
        LOG_ERROR << "pOrderServant is nullptr" << endl;
        return -1;
    }

    pOrderServant->async_modifyWalletBalance(NULL, req);
    LOG_DEBUG << "uid: " << req.uid << ", " << printTars(req)  << "}\n <<<< End \n\n\n\n" << endl;
    return 0;
}
// 获取用户账号余额(tg)
long OuterFactoryImp::selectWalletBalance(const long lUid)
{
    auto pOrderServant = getOrderServantPrx(lUid);
    if (!pOrderServant)
    {
        LOG_ERROR << "pOrderServant is nullptr" << endl;
        return 0;
    }

    return pOrderServant->selectWalletBalance(lUid);
}

//生产消息
int OuterFactoryImp::asyncClubGenMessage(const global::MessageReq &req)
{
    auto pGlobalServant = getGlobalServantPrx(req.lPlayerID);
    if (!pGlobalServant)
    {
        LOG_ERROR << "pGlobalServant is nullptr" << endl;
        return -1;
    }

    pGlobalServant->async_genMessage(NULL, req);

    LOG_DEBUG << printTars(req) << endl;
    return 0;
};

//日志入库
int OuterFactoryImp::asyncLog2DB(const int64_t uid, const DaqiGame::TLog2DBReq &req)
{
    auto pLog2DBServantPrx = getLog2DBServantPrx(uid);
    if (!pLog2DBServantPrx)
    {
        LOG_ERROR << "pLog2DBServantPrx is nullptr" << endl;
        return -1;
    }

    pLog2DBServantPrx->async_log2db(NULL, req);
    return 0;
}

//拆分字符串成整形
int OuterFactoryImp::splitInt(string szSrc, vector<int> &vecInt)
{
    split_int(szSrc, "[ \t]*\\|[ \t]*", vecInt);
    return 0;
}

//随机范围数
int OuterFactoryImp::nnrand(int max, int min)
{
    std::random_device rd;
    srand(rd());
    return min + rand() % (max - min + 1);
}

//游戏配置服务代理
const ConfigServantPrx OuterFactoryImp::getConfigServantPrx()
{
    if (!_ConfigServerPrx)
    {
        _ConfigServerPrx = Application::getCommunicator()->stringToProxy<config::ConfigServantPrx>(_ConfigServantObj);
        ROLLLOG_DEBUG << "Init _ConfigServantObj succ, _ConfigServantObj:" << _ConfigServantObj << endl;
    }

    return _ConfigServerPrx;
}

//数据库代理服务代理
const DBAgentServantPrx OuterFactoryImp::getDBAgentServantPrx(const long uid)
{
    if (!_DBAgentServerPrx)
    {
        _DBAgentServerPrx = Application::getCommunicator()->stringToProxy<dbagent::DBAgentServantPrx>(_DBAgentServantObj);
        ROLLLOG_DEBUG << "Init _DBAgentServantObj succ, _DBAgentServantObj:" << _DBAgentServantObj << endl;
    }

    if (_DBAgentServerPrx)
    {
        return _DBAgentServerPrx->tars_hash(uid);
    }

    return NULL;
}

//数据库代理服务代理
const DBAgentServantPrx OuterFactoryImp::getDBAgentServantPrx(const string key)
{
    if (!_DBAgentServerPrx)
    {
        _DBAgentServerPrx = Application::getCommunicator()->stringToProxy<dbagent::DBAgentServantPrx>(_DBAgentServantObj);
        ROLLLOG_DEBUG << "Init _DBAgentServantObj succ, _DBAgentServantObj:" << _DBAgentServantObj << endl;
    }

    if (_DBAgentServerPrx)
    {
        return _DBAgentServerPrx->tars_hash(tars::hash<string>()(key));
    }

    return NULL;
}

//PushServer代理
const PushServantPrx OuterFactoryImp::getPushServantPrx(const long uid)
{
    if (!_PushServerPrx)
    {
        _PushServerPrx = Application::getCommunicator()->stringToProxy<push::PushServantPrx>(_PushServantObj);
        ROLLLOG_DEBUG << "Init _PushServantObj succ, _PushServantObj:" << _PushServantObj << endl;
    }

    if (_PushServerPrx)
    {
        return _PushServerPrx->tars_hash(uid);
    }

    return NULL;
}

//HallServantPrx代理
const HallServantPrx OuterFactoryImp::getHallServantPrx(const long uid)
{
    if (!_HallServantPrx)
    {
        _HallServantPrx = Application::getCommunicator()->stringToProxy<hall::HallServantPrx>(_HallServantObj);
        ROLLLOG_DEBUG << "Init _HallServantObj succ, _HallServantObj:" << _HallServantObj << endl;
    }

    if (_HallServantPrx)
    {
        return _HallServantPrx->tars_hash(uid);
    }

    return NULL;
}

//GlobalServantPrx代理
const GlobalServantPrx OuterFactoryImp::getGlobalServantPrx(const long uid)
{
    if (!_GlobalServantPrx)
    {
        _GlobalServantPrx = Application::getCommunicator()->stringToProxy<global::GlobalServantPrx>(_GlobalServantObj);
        ROLLLOG_DEBUG << "Init _GlobalServantObj succ, _GlobalServantObj:" << _GlobalServantObj << endl;
    }

    if (_GlobalServantPrx)
    {
        return _GlobalServantPrx->tars_hash(uid);
    }

    return NULL;
}

//Log2DBServantPrx代理
const Log2DBServantPrx OuterFactoryImp::getLog2DBServantPrx(const long uid)
{
    if (!_Log2DBServantPrx)
    {
        _Log2DBServantPrx = Application::getCommunicator()->stringToProxy<DaqiGame::Log2DBServantPrx>(_Log2DBServantObj);
        ROLLLOG_DEBUG << "Init _Log2DBServantObj succ, _Log2DBServantObj : " << _Log2DBServantObj << endl;
    }

    if (_Log2DBServantPrx)
    {
        return _Log2DBServantPrx->tars_hash(uid);
    }

    return NULL;
}

//Log2DBServantPrx代理
const OrderServantPrx OuterFactoryImp::getOrderServantPrx(const long uid)
{
    if (!_OrderServantPrx)
    {
        _OrderServantPrx = Application::getCommunicator()->stringToProxy<order::OrderServantPrx>(_OrderServantObj);
        ROLLLOG_DEBUG << "Init _OrderServantPrx succ, _OrderServantObj : " << _OrderServantObj << endl;
    }

    if (_OrderServantPrx)
    {
        return _OrderServantPrx->tars_hash(uid);
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
