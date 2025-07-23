#ifndef __LOGCOMM_H__
#define __LOGCOMM_H__

#include <util/tc_logger.h>
#include <sstream>
#include <vector>
#include "servant/RemoteLogger.h"

//
using namespace tars;

//
#define ROLLLOG(level) (LOG->level() << "[" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "] ")
#define ROLLLOG_DEBUG (ROLLLOG(debug))
#define ROLLLOG_INFO (ROLLLOG(info))
#define ROLLLOG_WARN (ROLLLOG(warn))
#define ROLLLOG_ERROR (ROLLLOG(error))

#define FUNC_ENTRY(in) (ROLLLOG(debug) << ">>>> Enter " << __FUNCTION__ << "() in(" << in << ")" << endl)
#define FUNC_EXIT(out, ret) (ROLLLOG(debug) << "<<<< Exit " << __FUNCTION__ << "() out[" << out << "], ret = " << ret << endl)

#define FDLOG_ERROR (FDLOG("error") << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "|")
#define FDLOG_EXCEPT (FDLOG("except") << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "|")

//配置信息
#define FDLOG_CONFIG_INFO (FDLOG("config_info") << "|")

//游戏配置数据信息
#define FDLOG_GAME_CONFIG_INFO (FDLOG("game_config_info") << "|")

// 匿名空间替换static
namespace
{
    // 占位符替换,类似Python
    //Usage :	auto t = formatString("INSERT INTO {0} WHERE `id` = {1}, `value` = {0}, `name` = {2}", toStringVec(111, 999, "wzc"));
    std::vector<std::string> subsVec;
    std::string formatString(std::string target, const std::vector<std::string> &subs)
    {
        for (size_t i = 0; i < subs.size(); ++i)
        {
            std::string placeholders = "{" + std::to_string(i) + "}";
            const std::string &sub = subs.at(i);
            size_t pos = target.find(placeholders);
            size_t placeholders_len = placeholders.length();
            while (pos != std::string::npos)
            {
                target.replace(pos, placeholders_len, sub);
                pos = target.find(placeholders);
            }
        }
        return target;
    }

    template<typename T>
    std::vector<std::string> toStringVec(T t)
    {
        ostringstream os;
        os << t;
        subsVec.push_back(os.str());
        std::vector<std::string> result = subsVec;
        subsVec.clear();
        return result;
    }

    template<typename T, typename... Args>
    std::vector<std::string> toStringVec(T head, Args... args)
    {
        ostringstream os;
        os << head;
        subsVec.push_back(os.str());
        return toStringVec(args...);
    }
}
#endif



