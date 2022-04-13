#ifndef KYROSWEBSERVER_LOGSTREAM_H
#define KYROSWEBSERVER_LOGSTREAM_H

#include "Copyable.h"
#include <cstring>  //memcpy
#include <functional>

using namespace std;

namespace base{

class FixedBuffer{
public:
    FixedBuffer()
    {
        bufptr=_buffer;
        end = bufptr + sizeof _buffer;
    }
    void append(const char *str, size_t len)
    {
        if(static_cast<size_t>(end-bufptr)>len){
            memcpy(bufptr,str,len);
        }
        bufptr+=len;
    }
    int length() { return static_cast<int>(bufptr-_buffer);}
    int avail() {return static_cast<int>(end-bufptr);}
    int add(int len){
        if(len>avail())
            return -1;
        else{
            bufptr+=len;
            return length();
        }
    }

    void clear(){
        memset(_buffer,0,end-_buffer);
        reset();
    }
    void reset() {bufptr=_buffer;}

private:
    friend class LogStream;
    static const int kBufferSize=1023;
    char _buffer[kBufferSize+1];    //最后需要写入一个换行符，详见~LogStream()
    char* bufptr;
    char* end;
};

class LogStream: public noncopyable{
public:
    typedef std::function<void(const char* buf, int len)> OutputFunc;
    typedef std::function<void()> FlushFunc;

    LogStream();
    ~LogStream();
//    void setOutput(OutputFunc f);
//    void setFlush(FlushFunc f);

    // 重载<<
    LogStream& operator<<(bool v)
    {
        mBuffer.append(v ? "1" : "0", 1);
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
        mBuffer.append(&v, 1);
        return *this;
    }

    LogStream& operator<<(const char* str)
    {
        if (str)
        {
            mBuffer.append(str, strlen(str));
        }
        else
        {
            mBuffer.append("(null)", 6);
        }
        return *this;
    }

    LogStream& operator<<(const string& v)
    {
        mBuffer.append(v.c_str(), v.size());
        return *this;
    }

private:
    template<typename T> void formatInteger(T);
    static const int kMaxNumericSize = 32;
    FixedBuffer mBuffer;
    OutputFunc mOutput;
    FlushFunc mFlush;
};

}

#endif
