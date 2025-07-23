#include "ManageUserDB.h"
#include "LogComm.h"
#include "globe.h"
#include "servant/Application.h"
#include "SocialServant.h"
#include "ServiceDefine.h"
#include "SocialServer.h"
#include "TimeUtil.h"

// 获取玩家的地址
tars::Int32 ManageUserGetAddress(const DBAgentServantPrx prx, tars::Int64 uId, string &address)
{
    FUNC_ENTRY("");
    int iRet = 0;
    __TRY__

    if (!prx)
    {
        ROLLLOG_ERROR << "ManageUserGetAddress prx null!" << endl;
    	return -1;
    }

    dataproxy::TReadDataReq dataReq;
    dataReq.resetDefautlt();
    dataReq.keyName = I2S(E_REDIS_TYPE_HASH) + ":" + I2S(USER_STATE_ONLINE) + ":" + L2S(uId);
    dataReq.operateType = E_REDIS_READ;
    dataReq.clusterInfo.resetDefautlt();
    dataReq.clusterInfo.busiType = E_REDIS_PROPERTY;
    dataReq.clusterInfo.frageFactorType = E_FRAGE_FACTOR_USER_ID;
    dataReq.clusterInfo.frageFactor = uId;
    dataReq.paraExt.resetDefautlt();
    dataReq.paraExt.queryType = E_SELECT;

    vector<TField> fields;
    TField tfield;
    tfield.colArithType = E_NONE;
    tfield.colName = "gwaddr";
    tfield.colType = STRING;
    fields.push_back(tfield);
    tfield.colName = "gwcid";
    tfield.colType = STRING;
    fields.push_back(tfield);
    dataReq.fields = fields;

    TReadDataRsp dataRsp;
    iRet = prx->redisRead(dataReq, dataRsp);

    if (iRet != 0 || dataRsp.iResult != 0)
    {
        ROLLLOG_ERROR << "ManageUserGetAddress failed! iRet:" << iRet << ", iResult:" << dataRsp.iResult << endl;
        return -2;
    }

    ROLLLOG_DEBUG << "ManageUserGetAddress, iRet: " << iRet << ", dataRsp: " << printTars(dataRsp) << endl;

    string gwAddr = "";
    for (auto it = dataRsp.fields.begin(); it != dataRsp.fields.end(); ++it)
    {
        for (auto itField = it->begin(); itField != it->end(); ++itField)
        {
            if (itField->colName == "gwaddr")
            {
                gwAddr = itField->colValue;
            }
        }
    }

    address = gwAddr;

    __CATCH__

    FUNC_EXIT("", iRet);
    
    return iRet;
}

// 获取玩家名字
tars::Int32 ManageUserGetName(const HallServantPrx prx, long uId, string &name)
{
    FUNC_ENTRY("");
    int iRet = 0;
    __TRY__

    if (!prx)
    {
        ROLLLOG_ERROR << "ManageUserGetName prx null!" << endl;
        return -1;
    }

    userinfo::GetUserBasicReq basicReq;
    basicReq.uid = uId;
    userinfo::GetUserBasicResp basicResp;
    int iRet = prx->getUserBasic(basicReq, basicResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "ManageUserGetName failed! uid:" << uId << endl;
        return -2;
    }

    name = basicResp.userinfo.name;

    __CATCH__

    FUNC_EXIT("", iRet);
    
    return iRet;
}

// 获取玩家VIP等级
tars::Int32 ManageUserVipLevel(const HallServantPrx prx, long uId, int &vipLv)
{
    FUNC_ENTRY("");
    int iRet = 0;
    __TRY__

    if (!prx)
    {
        ROLLLOG_ERROR << "ManageUserVipLevel prx null!" << endl;
        return -1;
    }

    userinfo::GetUserBasicReq basicReq;
    basicReq.uid = uId;
    userinfo::GetUserBasicResp basicResp;
    int iRet = prx->getUserBasic(basicReq, basicResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "ManageUserVipLevel failed! uid:" << uId << endl;
        return -2;
    }

    vipLv = basicResp.userinfo.vip_level;

    __CATCH__

    FUNC_EXIT("", iRet);
    
    return iRet;
}

// 获取玩家的名字-头像-登出时间
tars::Int32 ManageUserGetNameAndAvatar(const HallServantPrx prx, long uId, string &nickname, string &avatar, long &logoutTime)
{
    FUNC_ENTRY("");
    int iRet = 0;
    __TRY__

    if (!prx)
    {
        ROLLLOG_ERROR << "ManageUserGetNameAndAvatar prx null!" << endl;
        return -1;
    }

    userinfo::GetUserBasicReq basicReq;
    basicReq.uid = uId;
    userinfo::GetUserBasicResp basicResp;
    int iRet = prx->getUserBasic(basicReq, basicResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "ManageUserGetNameAndAvatar failed! uid:" << uId << endl;
        return -2;
    }

    nickname = basicResp.userinfo.name;
    avatar = basicResp.userinfo.head;
    long loginTime = basicResp.userinfo.lastLoginTime; 
    logoutTime = basicResp.userinfo.lastLogoutTime;
    if (loginTime > logoutTime)
    {
        logoutTime = 0;
    }

    ROLLLOG_DEBUG << "ManageUserGetNameAndAvatar ok! uid:" << uId << ", login:" << basicResp.userinfo.lastLoginTime << ", logout:" << basicResp.userinfo.lastLogoutTime << endl;

    __CATCH__

    FUNC_EXIT("", iRet);
    
    return iRet;
}

// 获取玩家账号类型(tg)
tars::Int32 ManageUserGetAccountType(const HallServantPrx prx, long uId, bool &bInner)
{
    FUNC_ENTRY("");
    int iRet = 0;

    __TRY__

    if (!prx)
    {
        ROLLLOG_ERROR << "ManageUserGetAccountType prx null!" << endl;
        return -1;
    }

    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = uId;

    iRet = prx->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "ManageUserGetAccountType failed! uid:" << uId << endl;
        return -2;
    }

    bInner = true;
    if (userAccountResp.useraccount.bindTgId.length() != 0)
    {
        bInner = false;
    }

    __CATCH__
    return iRet;
}

// 检测玩家账号是否冻结
tars::Int32 ManageUserCheckAccount(const HallServantPrx prx, long uId, bool &bForbidden)
{
    FUNC_ENTRY("");
    int iRet = 0;

    __TRY__

    if (!prx)
    {
        ROLLLOG_ERROR << "ManageUserCheckAccount prx null!" << endl;
        return -1;
    }

    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = uId;

    iRet = prx->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "ManageUserCheckAccount failed! uid:" << uId << endl;
        return -2;
    }
    bForbidden = userAccountResp.useraccount.isForbidden == 0;

    __CATCH__
    return iRet;
}
