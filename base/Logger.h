//
// Created by 樱吹雪 on 2022/4/7.
//

#ifndef KYROSWEBSERVER_LOGGER_H
#define KYROSWEBSERVER_LOGGER_H

// fwrite()是原子操作
// 需要一个用户态的buffer 保存log的文本内容，并且一次性的刷入fd
// LOGGER数据流：logPacked->logstream->buffer->fwrite
// 需要构建的类是：Logger LogStream Buffer

#define LOG_TRACE if (base::Logger::logLevel() <= base::Logger::TRACE) \
            base::Logger(__FILE__, __LINE__, base::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (base::Logger::logLevel() <= base::Logger::DEBUG) \
            base::Logger(__FILE__, __LINE__, base::Logger::DEBUG, __func__).stream()
#define LOG_INFO   base::Logger(__FILE__, __LINE__, base::Logger::INFO, __func__).stream()
#define LOG_WARN   base::Logger(__FILE__, __LINE__, base::Logger::WARN, __func__).stream()
#define LOG_ERROR   base::Logger(__FILE__, __LINE__, base::Logger::ERROR, __func__).stream()

#include "LogStream.h"
#include "Timestamp.h"

namespace base{
class FileName{
public:
    template <unsigned int N>
    //const char (&s)[N]定义了以个指向字符串的常量引用
    FileName(const char (&s)[N])
    :mName(s){}
    constexpr const char* get(){
        const char *ret = strrchr(mName, '/');
        if(ret){
            return ret+1;
        }
        return ret;
    }
private:
    const char *mName;
};

class Logger {
public:
    static const int NUM_LOG_LEVELS = 5;
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
    };
    static LogLevel logLevel();
    LogStream& stream(){ return mStream; }
    Logger(FileName file, int line, enum LogLevel level, const char *func);
private:
    LogStream mStream;
};
}//namespace base
#endif //KYROSWEBSERVER_LOGGER_H
