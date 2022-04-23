#ifndef KYROSWEBSERVER_LOGSTREAM_H
#define KYROSWEBSERVER_LOGSTREAM_H

#include "Copyable.h"
#include "Buffer.h"
#include <cstring>  //memcpy
#include <functional>
#include <memory>

using namespace std;

namespace base{


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
    //使用unique_ptr的原因：
    // 1、避免直接调用构造函数生成匿名对象
    // 2、避免内存泄露
    FixedBuffer mBuffer;
    OutputFunc mOutput;
    FlushFunc mFlush;
};

}

#endif
