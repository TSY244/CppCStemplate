//
// Created by 12414 on 2023-07-19.
//

#ifndef CPPSIMP_CONSOLELOG_HPP
#define CPPSIMP_CONSOLELOG_HPP

#include <atomic>
#include <ctime>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

namespace TinyLog {
    using namespace std;

    class ConsoleLog {
    public:
        //日志等级的枚举
        enum class ELevel {
            eFatal = 3,  // 致命错误级别
            eError = 2,  // 错误级别
            eWarn = 1,  // 警告级别
            eInfo = 0,  // 信息级别
            eDebug = -1, // 调试级别
            eTrace = -2, // 追踪级别
        };
    private:
        const string currentTime() {
            time_t nowTime;
            char buffer[26] = {0};//缓冲区
            struct tm *tmInfo;

            nowTime = time(nullptr);
#pragma warning(suppress:4996)
            tmInfo = localtime(&nowTime);

            strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tmInfo);
            return buffer;
        }


        //定义日志函数的宏（充当与函数的构造器）
#define DEF_LOG_FUNC(funcName, PELvl) \
            void funcName(const string & sMsg, const string & sFilePath, \
                   const decltype(__LINE__) nLnNo, const string & sFunNm) { \
            if (eLvl > PELvl) { \
                return; \
            } else { /* do nothing */ } \
                                        \
            cout << "[" << currentTime() << "] " \
                << "[" << myName << "] " \
                << "[" << setw(5) << setfill(' ') << ""#funcName"" << "] " \
                << "[T " << this_thread::get_id() << "] " \
                << "[" << sFilePath << ":" << nLnNo << "] " \
                << "[" << sFunNm << "] " \
                << sMsg << endl; \
        }


    public:
        //构造函数
        ConsoleLog(string pName) : myName(pName) {}

        ConsoleLog() {}

        void setName(const string pName){
            myName=pName;
        }

        //设置当前的日志等级
        void setELevel(const ELevel PELvl) {
            eLvl = PELvl;
        }

        //获取当前的等级
        const ELevel getELevel() {
            return eLvl;
        }

        //定义日志函数，并且实现
        DEF_LOG_FUNC(fatal, ELevel::eFatal);

        DEF_LOG_FUNC(error, ELevel::eError);

        DEF_LOG_FUNC(warn, ELevel::eWarn);

        DEF_LOG_FUNC(info, ELevel::eInfo);

        DEF_LOG_FUNC(debug, ELevel::eDebug);

        DEF_LOG_FUNC(trace, ELevel::eTrace);


                // 定义用于输出不同级别日志信息的宏。
#define LOG_FATAL(logger, sMsg) logger.fatal((sMsg), __FILE__, __LINE__, __FUNCTION__);
#define LOG_ERROR(logger, sMsg) logger.error((sMsg), __FILE__, __LINE__, __FUNCTION__);
#define LOG_WARN(logger, sMsg)  logger.warn ((sMsg), __FILE__, __LINE__, __FUNCTION__);
#define LOG_INFO(logger, sMsg)  logger.info ((sMsg), __FILE__, __LINE__, __FUNCTION__);
#define LOG_DEBUG(logger, sMsg) logger.debug((sMsg), __FILE__, __LINE__, __FUNCTION__);
#define LOG_TRACE(logger, sMsg) logger.trace((sMsg), __FILE__, __LINE__, __FUNCTION__);


    private:
        string myName;
        atomic<ELevel> eLvl;
    };
}


#endif //CPPSIMP_CONSOLELOG_HPP
