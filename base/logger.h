#ifndef KYROSWEBSERVERS_LOGGER_H
#define KYROSWEBSERVER_LOGGER_H

#include "utils.h"
#include <cstring>  //memcpy
using namespace std;
#include <cstdio>

#define LOG_DEBUG(format, ...) printf(format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) printf(format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) printf(format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) printf(format, ##__VA_ARGS__)

// fwrite()是原子操作
// 需要一个用户态的buffer 保存log的文本内容，并且一次性的刷入fd
// LOGGER数据流：logPacked->logstream->buffer->fwrite
// 需要构建的类是：Logger LogStream FixedBuffer

namespace base{
const int kBufferSize=1000;

class FixedBuffer{
public:
    FixedBuffer()
    :bufptr(_buffer)
    {
        end = bufptr + sizeof _buffer;
    }
    void append(const char *str, size_t len)
    {
        if(static_cast<size_t>(end-bufptr)>len){
            memcpy(bufptr,str,len);
        }
        bufptr+=len;
    }
    int length() {return static_cast<int>(bufptr-_buffer);}
    int avail() {return static_cast<int>(end-bufptr);}

    void clear(){
        memset(_buffer,0,end-_buffer);
        reset();
    }
    void reset() {bufptr=_buffer;}

private:
    char _buffer[kBufferSize];
    char* bufptr;
    char* end;
};

class LogStream{
public:

    // 重载<<
    LogStream& operator<<(bool v)
    {
        buffer_.append(v ? "1" : "0", 1);
        return *this;
    }
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);

    LogStream& operator<<(const void*);

    LogStream& operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }
    LogStream& operator<<(double);
    // self& operator<<(long double);

    LogStream& operator<<(char v)
    {
        buffer_.append(&v, 1);
        return *this;
    }

private:
    template<typename T> void formatInteger(T);
    FixedBuffer buffer_;
};

}

#endif
