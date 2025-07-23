#ifndef _Manage_User_DB_H_
#define _Manage_User_DB_H_

#include "DBAgentServant.h"
#include "HallServant.h"

using namespace std;
using namespace dbagent;
using namespace hall;

// 获取玩家的地址
tars::Int32 ManageUserGetAddress(const DBAgentServantPrx prx, tars::Int64 uId, string &address);

// 获取玩家名字
tars::Int32 ManageUserGetName(const HallServantPrx prx, long uId, string &name);

// 获取玩家VIP等级
tars::Int32 ManageUserVipLevel(const HallServantPrx prx, long uId, int &vipLv);

// 获取玩家的名字-头像-登出时间
tars::Int32 ManageUserGetNameAndAvatar(const HallServantPrx prx, long uId, string &nickname, string &avatar, long &logoutTime);

// 获取玩家账号类型(tg) bInner 是否是内部账号(未绑定tg账号)
tars::Int32 ManageUserGetAccountType(const HallServantPrx prx, long uId, bool &bInner);

// 检测玩家账号是否冻结
tars::Int32 ManageUserCheckAccount(const HallServantPrx prx, long uId, bool &bWhite);

#endif