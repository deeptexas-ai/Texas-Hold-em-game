#ifndef _DB_OPERATOR_H_
#define _DB_OPERATOR_H_

#include <string>
#include <map>
#include <vector>

#include <util/tc_config.h>
#include <util/tc_mysql.h>
#include <util/tc_singleton.h>
#include <util/tc_autoptr.h>

///
#include <wbl/pthread_util.h>

//协议
#include "ConfigProto.h"
#include "ConfigServant.h"

//
using namespace std;
using namespace tars;
using namespace wbl;
using namespace config;

/**
*
* DB操作类
*/
class CDBOperator : public TC_HandleBase
{
public:
    /**
     *
    */
    CDBOperator();

    /**
     *
    */
    virtual ~CDBOperator();

public:
    //初始化
    int init();
    //初始化
    int init(const TC_DBConf &dbConf);
    //初始化
    int init(const string &dbhost, const string &dbuser, const string &dbpass, const string &dbname, const string &charset, const string &dbport);

public:
    //加载配置数据
    int loadConfig();

public:
    //加载游戏房间配置
    int loadGameRoomConfig();
    //打印游戏房间配置
    int printGameRoomConfig();
    //游戏房间配置
    const map<int, vector<GameRoom> > &getGameRoomConfig()
    {
        return mapGameRoomConfig;
    }

    //加载锦标赛配置
    int loadMatchRoomConfig();
    //打印锦标赛配置
    int printMatchRoomConfig();
    //锦标赛配置
    const map<int, MatchRoom> &getMatchRoomConfig()
    {
        return mapMatchRoomConfig;
    }

    //加载涨盲配置
    int loadMatchBlindConfig();
    //打印涨盲配置
    int printMatchBlindConfig();
    //涨盲配置
    const map<int, vector<MatchBlind> > &getMatchBlindConfig()
    {
        return mapMatchBlindConfig;
    }

    //加载报名消耗配置
    int loadEntryFeeConfig();
    //打印报名消耗配置
    int printEntryFeeConfig();
    //报名消耗配置
    const map<int, vector<EntryFee> > &getEntryFeeConfig()
    {
        return mapEntryFeeConfig;
    }

    //加载比赛奖励配置
    int loadMatchRewardConfig();
    //打印比赛奖励配置
    int printMatchRewardConfig();
    //比赛奖励配置
    const map<int, vector<MatchReward> > &getMatchRewardConfig()
    {
        return mapMatchRewardConfig;
    }
    //加载比赛奖励配置
    int loadMatchRewardConfigExt();
    //打印比赛奖励配置
    int printMatchRewardConfigExt();
    const map<int, vector<MatchReward> > &getMatchRewardConfigExt()
    {
        return mapMatchRewardConfigExt;
    }

    //加载奖励配置
    int loadRewardConfig();
    //打印奖励配置
    int printRewardConfig();
    //奖励配置
    const map<int, vector<Reward> > &getReward()
    {
        return mapRewardConfig;
    }

    //加载奖励配置
    int loadRewardConfigExt();
    //打印奖励配置
    int printRewardConfigExt();
    //奖励配置
    const map<int, vector<Reward> > &getRewardExt()
    {
        return mapRewardConfigExt;
    }

    //加载奖励道具配置
    int loadRewardPropsConfig();
    //打印奖励道具配置
    int printRewardPropsConfig();
    //奖励道具配置
    const map<int, map<int, RewardPropsExt > > &getRewardProps()
    {
        return mapRewardPropsConfig;
    }

    //加载奖励道具配置
    int loadRewardPropsConfigExt();
    //打印奖励道具配置
    int printRewardPropsConfigExt();
    //奖励道具配置
    const map<int, map<int, RewardPropsExt > > &getRewardPropsExt()
    {
        return mapRewardPropsConfigExt;
    }

    //加载道具配置
    int loadPropsConfig();
    //打印道具配置
    int printPropsConfig();

    //加载增值服务配置
    int loadServiceConfig();
    //打印增值服务配置
    int printServiceConfig();

    //加载APP版本信息
    int loadAppVersionInfo();
    //打印APP版本信息
    int printAppVersionInfo();

    //加载线下赛事门票配置
    int loadTicketInfo();
    //打印线下赛事门票配置
    int printTicketInfo();
    //加载道具配置信息
    int loadPropsInfo();
    //打印道具配置信息
    int printPorpsInfo();
    //加载道具兑换配置信息
    int loadPropsExchangeInfo();
    //打印道具兑换配置信息
    int printPropsExchangeInfo();
    // 加载签到配置
    int loadSignInConfig();
    //
    int printSignInConfig();
    // 加载金猪配置
    int loadGoldPigConfig();
    //
    int printGoldPigConfig();
    //
    int loadMallConfigRaw();
    //
    int loadGoldPigConfigRaw();
    //加载商城配置
    int loadMallConfig();
    //
    int printMallConfig();
    //加载房间删选配置
    int loadRoomFilterConfig();
    //
    int printRoomFilterConfig();
    //加载赠送道具配置信息
    int loadGivePropsCfgInfo();
    //打印赠送道具配置信息
    int printGivePorpsCfgInfo();
    //
    tars::Int32 MallConfigQuery(const config::MallConfigQueryReq &req, config::MallConfigQueryRsp &resp);
    //
    tars::Int32 MallConfigInsert(const config::MallConfigInsertReq &req, config::MallConfigInsertRsp &resp);
    //
    tars::Int32 MallConfigUpdate(const config::MallConfigUpdateReq &req, config::MallConfigUpdateRsp &resp);
    //
    tars::Int32 MallConfigDelete(const config::MallConfigDeleteReq &req);
    //道具配置
    //const map<int, vector<Props> >& getPropsConfig() { return mapPropsConfig; }
    const map<int, Props > &getPropsConfig()
    {
        return mapPropsConfig;
    }
    const map<int, BaseServiceConfigItem> &getServiceConfig()
    {
        return mapServiceConfig;
    }
    const vector<AppVersionInfo> &getAppVersionInfo()
    {
        return vecAppVersionInfo;
    }
    const vector<TicketConfigItem> &getTicketInfo()
    {
        return vecTicketInfo;
    }
    const map<int, TicketConfigItem> &getTicketInfoExt()
    {
        return mapTicketInfo;    //门票配置
    }
    const vector<PropsConfigItem> &getPropsInfo()
    {
        return vecPropsInfo;
    }
    const map<int, PropsConfigItem> &getPropsInfoMap()
    {
        return mapPropsInfo;
    }
    const map<int, PropsConfigItem> &getGivePropsCfgMap()
    {
        return mapGivePropsCfg;
    }
    const map<int, PropsExchangeConfigItem > &getExchangePropsConfig()
    {
        return exchangePropsInfo;
    }
    map<int, vector<SignInItem> > &getSignInConfig()
    {
        return mapSignInConfig;
    }
    const vector<GoldPigItem> &getGoldConfig()
    {
        return vecGoldPigConfig;
    }
    const map<string, RoomFilter> &getRoomFilter()
    {
        return mapRoomFilter;
    }
    // 加载任务配置
    int loadTaskConfig();
    //
    int printTaskConfig();
    //
    int printVecTaskNoviceCampConfig();
    //
    const map<int, TaskRefreshTimeItem> &getTaskRefreshTimeConfig()
    {
        return mapTaskRefreshTimeConfig;
    }

    const map<int, TaskItem> &getTaskConfig()
    {
        return mapTaskConfig;
    }

    const vector<int> &getTaskNoviceCampConfig()
    {
        return vecTaskNoviceCampConfig;
    }

    // const map<int, TaskRewardItem> &getTaskRewardConfig()
    // {
    //     return mapTaskRewardConfig;
    // }

    // 加载转盘配置
    int loadRotateConfig();
    //
    int printRotateConfig();
    //加载俱乐部等级配置
    int loadClubLevelConfig();
    //
    int printClubLevelConfig();
    //
    const map<int, ClubLevelItem> &getClubLevelConfig()
    {
        return mapClubLevelConfig;
    }
    //加载机器人行为配置
    int loadRobotActionConfig();
    //
    int printRobotActionConfig();
    //加载奖池配置
    int loadJackPoolConfig();
    //
    int printJackPoolConfig();
    //
    const map<std::string, RobotjackPool> &getJackPoolConfig()
    {
        return mapJackPookConfig;
    };
    //加载必胜名单配置
    int loadWinListConfig();
    //
    int printWinListConfig();
    //
    int getWinListConfig(string roomid, RoomWinnerList &cfg);
    //
    const vector<tars::Int64> &getWinlist()
    {
        return winlist;
    }
    //加载机器人配置
    int loadRobotConfig();
    //
    int printRobotConfig();
    //
    int getRobotActionConfigByUid(int uid, RobotConfig &robot_active);
    //
    const vector<tars::Int64> &getRobotList()
    {
        return robotList;
    }
    //
    const map<string, vector<tars::Int64>> &getRoomRobotList()
    {
        return roomRobotList;
    }
    //转轮配置
    int loadRunnerConfig();
    //
    int printRunnerConfig();
    //
    RunnerConfig &getRunnerList()
    {
        return runnerconfig;
    }
    //加载赛事房间积分奖励组配置
    int loadGradeIntegralCfg();
    //打印赛事房间积分奖励组配置
    int printGradeIntegralCfg();
    //赛事房间积分奖励组配置
    const map<string, int> &getGradeIntegralCfg()
    {
        return mapGradeIntegralCfg;
    }
    //加载段位积分奖励组配置明细
    int loadGradeRankIntegralCfg();
    //打印段位积分奖励组配置明细
    int printGradeRankIntegralCfg();
    //段位积分奖励组配置明细
    const map<int, map<int, vector<GradeRankIntegral> > > &getGradeRankIntegralCfg()
    {
        return mapGradeRankIntegralCfg;
    }
    //加载段位阶级更新配置
    int loadGradeLevelUpdateCfg();
    //打印段位阶级更新配置
    int printGradeLevelUpdateCfg();
    //段位阶级更新配置
    const map<int, map<int, LevelUpdateCfg> > &getGradeLevelUpdateCfg()
    {
        return mapGradeLevelUpdateCfg;
    }
    //加载赛季奖励配置
    int loadSeasonRewardsCfg();
    //打印赛季奖励配置
    int printSeasonRewardsCfg();
    //赛季奖励配置
    const map<int, SeasonRewards > &getSeasonRewardsCfg()
    {
        return mapSeasonRewardsCfg;
    }
    //加载赛季段位奖励组
    int loadSeasonGradeRewardsGroup();
    //打印赛季段位奖励组
    int printSeasonGradeRewardsGroup();
    //赛季段位奖励组
    const map<int, map<int, GradeRewardsGroup > > &getSeasonGradeRewardsGroup()
    {
        return mapSeasonGradeRewardsGroup;
    }
    //加载赛季排名奖励组
    int loadSeasonRankRewardsGroup();
    //打印赛季排名奖励组
    int printSeasonRankRewardsGroup();
    //赛季排名奖励组
    const map<int, map<int, RankRewardsGroup > > &getSeasonRankRewardsGroup()
    {
        return mapSeasonRankRewardsGroup;
    }
    //加载赛季奖励组配置
    int loadSeasonRewardsGroupCfg();
    //打印赛季奖励组配置
    int printSeasonRewardsGroupCfg();
    //赛季奖励组配置
    const map<int, SeasonRewardsGroupCfg > &getSeasonRewardsGroupCfg()
    {
        return mapSeasonRewardsGroupCfg;
    }
    //加载玩家等级经验配置
    int loadUserLevelExperience();
    //打印玩家等级经验配置
    int printUserLevelExperience();
    //玩家等级经验配置
    const map<int, UserLevelExperience > &getUserLevelExperience()
    {
        return mapUserLevelExperience;
    }
    //加载房间经验配置
    int loadRoomExperienceCfg();
    //打印房间经验配置
    int printRoomExperienceCfg();
    //房间经验配置
    const std::vector<RoomExperienceCfg> &getRoomExperienceCfg()
    {
        return vecRoomExperienceCfg;
    }
    //加载网关信息配置
    int loadRounterCfg();
    //打印网关信息配置
    int printRounterCfg();
    //获取网关信息配置
    const map<int, RounterCfg> &getRounterCfg()
    {
        return mapRounterCfg;
    }
    //加载活动配置
    int loadActivitiesCfg();
    //打印活动配置
    int printActivitiesCfg();
    //活动配置
    const map<int, ActivitiesCfg > &getActivitiesCfg()
    {
        return mapActivitiesCfg;
    }
    //加载通用配置
    int loadGeneralValueConfig();
    //打印通用配置
    int printGeneralValueConfig();
    //通用配置
    const map<int, map<int, GeneralConfig > > &getGeneralValueConfig()
    {
        return mapGeneralConfig;
    }
    //加载充值相关配置
    int loadRechargeConfig();
    //打印充值相关配置
    int printRechargeConfig();
    //限时优惠充值配置
    const map<int, NoviceRecharge > &getNoviceRecharge()
    {
        return mapNoviceRecharge;
    }
    //超值优惠充值配置
    const map<int, FirstRecharge > &getFirstRecharge()
    {
        return mapFirstRecharge;
    }
    //每日优惠充值任务配置
    const map<int, DayRechargeTask > &getDayRechargeTask()
    {
        return mapDayRechargeTask;
    }
    //加载系统邮件信息配置
    int loadSysMailInfoCfg();
    //打印系统邮件信息配置
    int printSysMailInfoCfg();
    //系统邮件信息配置
    const map<int, SysMailInfo > &getSysMailInfoCfg()
    {
        return mapSysMailInfoCfg;
    }
    //加载邀请奖励信息配置
    int loadShareInfoCfg();
    //邀请奖励信息配置
    const map<int, ListShareInfo> &getShareInfoCfg()
    {
        return mapShareInfoCfg;
    }

    //加载兑换码配置map<int, ExchangeCodeConfig> mapExchangeCodeCfg
    int loadExchangeCodeCfg();
    //获取兑换码配置
    const map<int, ExchangeCodeConfig> &getExchangeCodeConfig()
    {
        return mapExchangeCodeCfg;
    }

    int loadPrivateRoomBlind();
    //
    int printPrivateRoomBlind();
    //
    const map<int, map<int, PrivateRoomBlind>> &getPrivateRoomBlind()
    {
        return mapPrivateRoomBlind;
    }
    //
    int loadPrivateRoomCost();
    //
    int printPrivateRoomCost();
    //
    string getMemberVariables(const map<string, string> &src, const string &name);
    //
    int loadAllRobotConfig(int room_type);

    int loadAIGamblerCfg();

    int loadDebugCardCfg();

    int loadScrathCfg();

    int loadWatchAdCfg();
    void printWatchAdCfg();

    int loadBroadcastCfg();
    void printBroadcastCfg();

    int loadPreciousBoxCfg();
    void printPreciousBoxCfg();

    int loadLuckyWheelCfg();
    void printLuckyWheelCfg();

    int loadJackpotCfg();
    void printJackpotCfg();

    int loadNicknameCfg();
    void printNicknameCfg();

    int outUid2intUid(map<long, long>& m_Robot2Uid);
    //
    const RoomSo::RobotAllAction &getAllRobotConfig(int room_type)
    {
        loadAllRobotConfig(room_type);
        return robotAllAction;
    }
    //
    const map<int, PrivateRoomCost> &getPrivateRoomCost()
    {
        return mapPrivateRoomCost;
    }
    //
    const map<int, map<int, map<int, map<int, MallGoodsDetail>>>> &getMallConfig()
    {
        return mallConfig;
    }
    //
    const vector<map<string, string>> &getMallConfigRaw()
    {
        return mallConfigRaw;
    }
    //
    const vector<map<string, string>> &getGoldPigConfigRaw()
    {
        return goldPigConfigRaw;
    }
    //
    const vector<map<string, string>> &getRotateEditConfig()
    {
        return mapRotateEdit;
    }
    //
    const vector<map<string, string>> &getRotateFreeConfig()
    {
        return mapRotateFree;
    }
    //
    const vector<map<string, string>> &getRotateControlConfig()
    {
        return mapRotateControl;
    }

    const map<int, map<int, std::string>> &getDebugCardConfig()
    {
        return mapDebugCard;
    }

    const map<int, ScratchConfig> &getScratchCfg()
    {
        return mapScratchCfg;
    }

    const map<int, WatchAdConfig> &getWatchAdCfg()
    {
        return mapWatchAdCfg;
    }
    const map<int, BroadcastConfig> &getBroadcastCfg()
    {
        return mapBroadcastCfg;
    }
    const map<int, PreciousBoxConfig> &getPreciousBoxCfg()
    {
        return mapPreciousBoxCfg;
    }
    const map<int, vector<LuckyWheelConfig>> &getLuckyWheelCfg()
    {
        return mapLuckyWheelCfg;
    }
    const map<int, vector<JackpotConfig>> &getJackpotCfg()
    {
        return mapJackpotCfg;
    }
    const map<int, vector<NicknameConfig>> &getNicknameCfg()
    {
        return mapNicknameConfig;
    }

private:
    //游戏房间配置
    map<int, vector<GameRoom> > mapGameRoomConfig;
    //锦标赛房间配置
    map<int, MatchRoom> mapMatchRoomConfig;
    //涨盲配置
    map<int, vector<MatchBlind> > mapMatchBlindConfig;
    //报名费配置
    map<int, vector<EntryFee> > mapEntryFeeConfig;
    //比赛奖励配置,key:rewardID
    map<int, vector<MatchReward> > mapMatchRewardConfig;
    //比赛奖励配置，key:batchid
    map<int, vector<MatchReward> > mapMatchRewardConfigExt;
    //奖励配置,key:rewardid
    map<int, vector<Reward> > mapRewardConfig;
    //奖励配置
    map<int, vector<Reward> > mapRewardConfigExt;
    //奖励道具配置,key:rewardID:rank
    map<int, map<int, RewardPropsExt > > mapRewardPropsConfig;
    //奖励道具配置,key:rewardID:rank
    map<int, map<int, RewardPropsExt > > mapRewardPropsConfigExt;
    //道具配置
    map<int, Props> mapPropsConfig;
    //增值服务配置
    map<int, BaseServiceConfigItem> mapServiceConfig;
    //APP版本信息
    vector<AppVersionInfo> vecAppVersionInfo;
    //门票配置
    vector<TicketConfigItem> vecTicketInfo;
    //门票配置
    map<int, TicketConfigItem> mapTicketInfo;
    //道具配置信息
    vector<PropsConfigItem> vecPropsInfo;
    //道具配置信息
    map<int, PropsConfigItem> mapPropsInfo;
    //赠送道具配置信息
    map<int, PropsConfigItem> mapGivePropsCfg;
    //兑换配置,id|item
    map<int, PropsExchangeConfigItem> exchangePropsInfo;
    //签到配置
    map<int, vector<SignInItem> > mapSignInConfig;
    //金猪配置
    vector<GoldPigItem> vecGoldPigConfig;
    // 未加工的金猪配置数据
    vector<map<string, string>> goldPigConfigRaw;
    //俱乐部等级配置
    map<int, ClubLevelItem> mapClubLevelConfig;
    //奖池配置
    map<std::string, RobotjackPool> mapJackPookConfig;
    //必胜名单配置
    map<std::string, RoomWinnerList> mapWinListConfig;
    //胜率配置
    vector<tars::Int64> winlist;
    //机器人行为配置
    map<int, vector<RobotActionItem> > mapRobotActionConfig;
    //机器人配置
    map<int, RobotItem> mapRobotConfig;
    //机器人总表
    vector<tars::Int64> robotList;
    //机器人房间分类表
    map<string, map<int, RobotItem>> mapRoomRobotConfig;
    //房间机器人总表
    map<string, vector<tars::Int64>> roomRobotList;
    //
    map<string, RoomFilter>  mapRoomFilter;
    //
    RunnerConfig runnerconfig;
    // 任务配置key:taskType
    map<int, TaskRefreshTimeItem> mapTaskRefreshTimeConfig;
    //key: taskId
    map<int, TaskItem>  mapTaskConfig;
    //新手训练营配置index:day
    vector<int> vecTaskNoviceCampConfig;
    // 商城配置
    map<int, map<int, map<int, map<int, MallGoodsDetail>>>> mallConfig;
    // 未加工的配置数据
    vector<map<string, string>> mallConfigRaw;
    //转盘配置
    vector<map<string, string>> mapRotateEdit;
    //
    vector<map<string, string>> mapRotateFree;
    //
    vector<map<string, string>> mapRotateControl;
    //赛事房间积分奖励组配置,key:room_id,value:integral_id
    map<string, int> mapGradeIntegralCfg;
    //段位积分奖励组配置明细,integral_id|grade|rankinfo
    map<int, map<int, vector<GradeRankIntegral> > > mapGradeRankIntegralCfg;
    //段位阶级更新配置,grade|level|updatecfginfo
    map<int, map<int, LevelUpdateCfg> > mapGradeLevelUpdateCfg;
    //赛季奖励配置,reward_id|rewardinfo
    map<int, SeasonRewards > mapSeasonRewardsCfg;
    //赛季段位奖励组,grade_id|grade|rewardinfo
    map<int, map<int, GradeRewardsGroup > > mapSeasonGradeRewardsGroup;
    //赛季排名奖励组,rank_id|rank|rewardinfo
    map<int, map<int, RankRewardsGroup > > mapSeasonRankRewardsGroup;
    //赛季奖励组配置,season_id|groupinfo
    map<int, SeasonRewardsGroupCfg > mapSeasonRewardsGroupCfg;
    //玩家等级经验配置,level|levelinfo
    map<int, UserLevelExperience > mapUserLevelExperience;
    //房间经验配置,room_id|experienceinfo
    vector<RoomExperienceCfg > vecRoomExperienceCfg;
    //网关配置
    map<int, RounterCfg > mapRounterCfg;//网关信息配置,id|addr|port|state|online|desc
    //活动配置
    map<int, ActivitiesCfg > mapActivitiesCfg;//type|cfginfo
    //通用配置
    map<int, map<int, GeneralConfig > > mapGeneralConfig;//type|sub_type|generalconfig
    //限时优惠充值配置,recharge_id|rewardinfo
    map<int, NoviceRecharge > mapNoviceRecharge;
    //超值优惠充值配置,recharge_id|rewardinfo
    map<int, FirstRecharge > mapFirstRecharge;
    //每日优惠充值任务配置,recharge_id|rewardinfo,tasklist
    map<int, DayRechargeTask > mapDayRechargeTask;
    //房间盲注信息
    map<int, map<int, PrivateRoomBlind>> mapPrivateRoomBlind;
    //房间消耗
    map<int, PrivateRoomCost> mapPrivateRoomCost;
    //邮件配置
    map<int, SysMailInfo > mapSysMailInfoCfg;//mail_id|mailInfo
    //机器人行为配置
    RoomSo::RobotAllAction robotAllAction;
    //邀请奖励配置
    map<int, ListShareInfo> mapShareInfoCfg;
    //兑换码配置
    map<int, ExchangeCodeConfig> mapExchangeCodeCfg;//id-info
    //debug card
    map<int, map<int, std::string>> mapDebugCard;
    //刮刮卡配置
    map<int, ScratchConfig> mapScratchCfg;
    //观看广告配置
    map<int, WatchAdConfig> mapWatchAdCfg;
    //广播配置
    map<int, BroadcastConfig> mapBroadcastCfg;
    //宝箱配置
    map<int, PreciousBoxConfig> mapPreciousBoxCfg;//key:betId
    //转盘配置
    map<int, vector<LuckyWheelConfig>> mapLuckyWheelCfg;
    //jackpot等级配置
    map<int, vector<JackpotConfig>> mapJackpotCfg;
    //VIP信息配置
    map<int, SysVipConfig> mapSysVipCfg;
    //常量信息配置
    map<string, SysConstConfig> mapSysConstCfg;
    //俱乐部基础信息配置
    map<int, ClubInfoConfig> mapClubInfoCfg;
    //俱乐部肓注信息配置
    map<int, ClubBlindConfig> mapClubBlindCfg;
     //俱乐部座位信息配置
    map<int, ClubSeatConfig> mapClubSeatCfg;
    //俱乐部等级信息配置
    map<int, ClubLevelConfig> mapClubLevelCfg;
    //俱乐部基金信息配置
    map<int, ClubCoinConfig> mapClubCoinCfg;
    //俱乐部时间花费信息配置
    map<int, ClubTimeCostConfig> mapClubTimeCostCfg;
    //俱乐部保险保证金等级信息配置
    map<int, ClubLevelCoinConfig> mapClubLevelCoinCfg;
    //昵称配置
    map<int, vector<NicknameConfig>> mapNicknameConfig; //key（0：未知， 1：男， 2：女）
    //系统通知
    map<int, vector<string>> mapSysNotice;// 系统通知
    //联盟等级配置
    map<int, UnionLevelConfig> mapUnionLevelCfg;
    //牛仔信息配置
    map<long, CowboyInfoConfig> mapCowboyInfoCfg;
    //牛仔匹配赔率配置
    map<long, CowboyAreaConfig> mapCowboyAreaCfg;
    //大菠萝配置
    map<long, PineAppleInfoConfig> mapPineAppleInfoCfg;
    //玩家背包配置
    map<long, UserBagInfoConfig> mapUserBagInfoCfg;
    
private:
    //读写锁，防止数据脏读
    wbl::ReadWriteLocker m_rwlock;
    //mysql操作对象
    TC_Mysql m_mysqlObj;

    // 有时间放置到一个类里面
public:
    //
    int addPropsConfig(const config::PropsConfigInsertReq &req, PropsConfigInsertRsp &resp);
    //
    int deletePropsConfig(const config::PropsConfigDeleteReq &req);
    //
    int queryPropsConfig(const config::PropsConfigQueryReq &req, config::PropsConfigQueryRsp &resp);
    //
    int updatePropsConfig(const config::PropsConfigUpdateReq &req, config::PropsConfigUpdateRsp &resp);
public:
    const map<int, SysVipConfig> &getSysVipCfg()
    {
        return mapSysVipCfg;
    }
    const map<string, SysConstConfig> &getSysConstCfg()
    {
        return mapSysConstCfg;
    }
    const map<int, ClubInfoConfig> &getClubInfoCfg()
    {
        return mapClubInfoCfg;
    }
    const map<int, ClubBlindConfig> &getClubBlindCfg()
    {
        return mapClubBlindCfg;
    }
    const map<int, ClubSeatConfig> &getClubSeatCfg()
    {
        return mapClubSeatCfg;
    }
    const map<int, ClubLevelConfig> &getClubLevelCfg()
    {
        return mapClubLevelCfg;
    }
    const map<int, ClubCoinConfig> &getClubCoinCfg()
    {
        return mapClubCoinCfg;
    }
    const map<int, ClubTimeCostConfig> &getClubTimeCostCfg()
    {
        return mapClubTimeCostCfg;
    }
    const map<int, ClubLevelCoinConfig> &getClubLevelCoinCfg()
    {
        return mapClubLevelCoinCfg;
    }
    const map<int, vector<string>> &getSysNoticeCfg()
    {
        return mapSysNotice;
    }
    const map<int, UnionLevelConfig> &getUnionLevelCfg()
    {
        return mapUnionLevelCfg;
    }
    const map<long, CowboyInfoConfig> &getCowboyInfoCfg()
    {
        return mapCowboyInfoCfg;
    }
    const map<long, CowboyAreaConfig> &getCowboyAreaCfg()
    {
        return mapCowboyAreaCfg;
    }
    const map<long, PineAppleInfoConfig> &getPineAppleInfoCfg()
    {
        return mapPineAppleInfoCfg;
    }
    const map<long, UserBagInfoConfig> &getUserBagInfoCfg()
    {
        return mapUserBagInfoCfg;
    }
public:
    // 加载vip卡信息配置
    int loadSysVipCfg();
    void printSysVipCfg();
    // 加载常量表
    int loadSysConstCfg();
    void printSysConstCfg();
    // 加载俱乐部基础信息配置
    int loadClubInfoCfg();
    void printClubInfoCfg();
    // 加载俱乐部肓注信息配置
    int loadClubBlindCfg();
    void printClubBlindCfg();
    // 加载俱乐部座位信息配置
    int loadClubSeatCfg();
    void printClubSeatCfg();
    // 加载俱乐部等级信息配置
    int loadClubLevelCfg();
    void printClubLevelCfg();
    // 加载俱乐部基金信息配置
    int loadClubCoinCfg();
    void printClubCoinCfg();
    // 加载俱乐部时间花费信息配置
    int loadClubTimeCostCfg();
    void printClubTimeCostCfg();
    // 加载俱乐部保险保证金等级信息配置
    int loadClubLevelCoinCfg();
    void printClubLevelCoinCfg();
    // 系统消息
    int loadSysNoticeCfg();
    void printSysNoticeCfg();
    // 加载联盟等级表
    int loadUnionLevelCfg();
    void printUnionLevelCfg();
    // 加载牛仔信息配置表
    int loadCowboyInfoCfg();
    void printCowboyInfoCfg();
    // 加载牛仔区域赔率配置表
    int loadCowboyAreaCfg();
    void printCowboyAreaCfg();
    // 加载大菠萝配置表
    int loadPineAppleInfoCfg();
    void printPineAppleInfoCfg();
    // 加载玩家背包
    int loadUserBagInfoCfg();
    void printUserBagInfoCfg();

};

//singleton
typedef TC_Singleton<CDBOperator, CreateStatic, DefaultLifetime> DBOperatorSingleton;

//ptr
typedef TC_AutoPtr<CDBOperator> CDBOperatorPtr;

#endif


