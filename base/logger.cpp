//
// Created by 樱吹雪 on 2022/4/7.
//

#include "Logger.h"
#include "Thread.h"
namespace base {

const char *LogLevelName[Logger::NUM_LOG_LEVELS]
={
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
};

Logger::LogLevel initLogLevel()
{
    if(getenv("SET_LOG_TRACE"))
        return Logger::TRACE;
    else if(getenv("SET_LOG_DEBUG"))
        return Logger::DEBUG;
    else
        return Logger::INFO;
}

Logger::LogLevel gLogLevel=initLogLevel();

Logger::LogLevel Logger::logLevel() {
    return gLogLevel;
}

Logger::Logger(FileName file, int line, enum LogLevel level, const char *func) {
    Timestamp now(Timestamp::now());
    mStream<<now.toFormattedString(false)<<" ";
    mStream<<"Tid:"<<CurrentThread::getStr();
    mStream <<" ["<< LogLevelName[level];
    mStream <<"] "<< file.get();
    mStream << " line:" << line;
    mStream << "  func:" << func<<" ";
}

}//namespace base
