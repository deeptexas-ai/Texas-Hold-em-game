#ifndef _OUTER_FACTORY_IMP_H_
#define _OUTER_FACTORY_IMP_H_

#include <string>
#include <map>

//
#include "servant/Application.h"
#include "globe.h"
#include "OuterFactory.h"
#include "util/tc_hash_fun.h"
#include "UserInfoProto.h"

//wbl
#include <wbl/regex_util.h>

//配置服务
#include "ConfigServant.h"
#include "DBAgentServant.h"
#include "PushServant.h"
#include "HallServant.h"
#include "GlobalServant.h"
#include "Log2DBServant.h"
#include "OrderServant.h"

//
using namespace config;
using namespace dataproxy;
using namespace dbagent;
using namespace wbl;
using namespace push;
using namespace hall;
using namespace global;
using namespace DaqiGame;
using namespace order;

//时区
#define ONE_DAY_TIME (24*60*60)
#define ZONE_TIME_OFFSET (8*60*60)

struct MailConfig
{
    std::string sTitle;//邮件主题
    std::string sContent;//邮件内容
};

//
class OuterFactoryImp;
typedef TC_AutoPtr<OuterFactoryImp> OuterFactoryImpPtr;

/**
 * 外部工具接口对象工厂
 */
class OuterFactoryImp : public OuterFactory
{
private:
    /**
     *
    */
    OuterFactoryImp();

    /**
     *
    */
    ~OuterFactoryImp();

    /**
     *
     */
    friend class SocialServantImp;

    /**
     *
     */
    friend class SocialServer;

public:
    //框架中用到的outer接口(不能修改):
    const OuterProxyFactoryPtr &getProxyFactory() const
    {
        return _pProxyFactory;
    }
    //
    tars::TC_Config &getConfig() const
    {
        return *_pFileConf;
    }

public:
    //加载配置
    void load();
    //代理配置
    void readPrxConfig();
    //输出代理信息
    void printPrxConfig();
    //邮件配置
    void readMailConfig();
    //输出邮件配置
    void printMailConfig();

    // 俱乐部等级配置
    void loadClubLevelConfig();
    // 输出俱乐部等级配置
    void printClubLevelConfig(); 
    // 获取俱乐部等级配置
    int getClubLevelConfig(int iId, config::ClubLevelConfig &cfg);
    // 获取升级俱乐部等级配置
    int getUpClubLevelConfig(long exp, config::ClubLevelConfig &cfg);
    // 获取降级俱乐部等级配置
    int getDownClubLevelConfig(long exp, config::ClubLevelConfig &cfg);

    // 俱乐部商户等级配置
    void loadClubLevelCoinConfig();
    // 输出俱乐部商户等级配置
    void printClubLevelCoinConfig();
    // 获取俱乐部商户等级配置
    int getClubLevelCoinConfig(int iLevel, config::ClubLevelCoinConfig &cfg);
    // 获取俱乐部商户最大等级
    int getClubLevelCoinMaxLevel();


    // 俱乐部基金配置
    void loadClubCoinConfig();
    // 输出俱乐部基金配置
    void printClubCoinConfig(); 
    // 获取俱乐部基金配置
    int getClubCoinConfig(int iId, config::ClubCoinConfig &cfg);

    // VIP信息配置
    void loadSysVipConfig();
    // 输出VIP信息配置
    void printSysVipConfig();
    // 获取VIP信息配置
    int getSysVipConfig(int iLevel, config::SysVipConfig &cfg);

    // CONST信息配置
    void loadSysConstConfig();
    // 输出CONST信息配置
    void printSysConstConfig();
    // 获取CONST信息配置
    int getSysConstConfig(string sName, config::SysConstConfig &cfg);

    // 联盟等级配置
    void loadUnionLevelConfig();
    // 输出联盟等级配置
    void printUnionLevelConfig();
    // 获取联盟等级配置
    int getUnionLevelConfig(int iLevel, config::UnionLevelConfig &cfg);

private:
    //
    void createAllObject();
    //
    void deleteAllObject();

public:
    //获取配置代理
    const ConfigServantPrx getConfigServantPrx();
    //获取DB代理
    const DBAgentServantPrx getDBAgentServantPrx(const long uid);
    //获取DB代理
    const DBAgentServantPrx getDBAgentServantPrx(const string key);
    //获取PushServer代理
    const PushServantPrx getPushServantPrx(const long uid);
    //获取HallServantPrx代理
    const HallServantPrx getHallServantPrx(const long uid);
    //获取GlobalServantPrx代理
    const GlobalServantPrx getGlobalServantPrx(const long uid);
    //获取Log2DBServantPrx代理
    const Log2DBServantPrx getLog2DBServantPrx(const long uid);
    //获取OrderServantPrx代理
    const OrderServantPrx getOrderServantPrx(const long uid);
    //获取
    //格式化时间
    string GetTLogTimeFormat();
    //格式化自定义时间
    string GetCustomTimeFormat(int time);
    //获取自定义秒数
    int GetCustomTimeTick(const string &str);
    //获取自定义年月日
    int GetCustomDateFormat(int time);
    //推送消息
    void asyncRequest2Push(const long uid, const push::PushMsgReq &msg);
    //邮件通知
    int asyncClubMailNotify(const long lUid, const int type, const int mailType, const long clubId, std::vector<string> vParam);
    //
    int async2ModifyUWealth(const userinfo::ModifyUserWealthReq &req);
    int async2ModifyUWalletBalance(const order::ModifyWalletBalanceReq &req);
    long selectWalletBalance(const long lUid);
    // 生产消息
    int asyncClubGenMessage(const global::MessageReq &req);

    // 日志入库
    int asyncLog2DB(const int64_t uid, const DaqiGame::TLog2DBReq &req);
public:
    //读取最大好友数量
    // void readMaxFriendsCount();
    //输出参数配置信息
    // void printMaxFriendsCount();
    //获取好友数量
    int getMaxFriendsCount();
    //加载通用配置
    void readGeneralConfigResp();
    //
    void printGeneralConfigResp();
    //拆分字符串成整形
    int splitInt(string szSrc, vector<int> &vecInt);

    //读取创建俱乐部费用
    void readCreateClubCost();
    //获取创建俱乐部费用
    long getCreateClubCost();

public:
    //范围随机数
    int nnrand(int max, int min = 0);

private:
    //
    wbl::ReadWriteLocker m_rwlock;
    //
    tars::TC_Config *_pFileConf;
    //
    OuterProxyFactoryPtr _pProxyFactory;
    //
    std::string _ConfigServantObj;
    //
    ConfigServantPrx _ConfigServerPrx;
    //
    std::string _DBAgentServantObj;
    //
    DBAgentServantPrx _DBAgentServerPrx;
    //
    std::string _PushServantObj;
    //
    PushServantPrx _PushServerPrx;
    //
    std::string _HallServantObj;
    //
    HallServantPrx _HallServantPrx;
    //
    std::string _GlobalServantObj;
    //
    GlobalServantPrx _GlobalServantPrx;
    //
    std::string _Log2DBServantObj;
    //
    Log2DBServantPrx _Log2DBServantPrx;
    //
    std::string _OrderServantObj;
    //
    OrderServantPrx _OrderServantPrx;

private:
    //最大好友数量
    // int maxFriendsCount;
    //创建俱乐部费用
    long m_create_club_cost;
    //通用配置
    config::ListGeneralConfigResp listGeneralConfigResp;
    //邮件配置
    std::map<int, MailConfig> mapMailConfig;
    //俱乐部等级信息配置
    map<int, config::ClubLevelConfig> m_mapClubLevelCfg;
    //俱乐部商户等级配置
    map<int, config::ClubLevelCoinConfig> m_mapClubLevelCoinCfg;
    //俱乐部基金信息配置
    map<int, config::ClubCoinConfig> m_mapClubCoinCfg;
    //VIP信息配置
    map<int, config::SysVipConfig> m_mapSysVipCfg;
    //CONST信息配置
    map<string, config::SysConstConfig> m_mapSysConstCfg;
    //联盟等级配置
    map<int, config::UnionLevelConfig> m_mapUnionLevelCfg;
    //俱乐部商户等级最大值
    int m_club_max_merchant_level;
};

////////////////////////////////////////////////////////////////////////////////
#endif


