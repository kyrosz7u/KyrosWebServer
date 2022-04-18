//
// Created by 樱吹雪 on 2022/2/4.
//

#include "LogStream.h"
#include <algorithm>

#include <iostream>

using namespace std;
using namespace base;

void defaultOutput(const char *buf, int len){
    size_t n = fwrite(buf, 1, len, stdout);
}

void defaultFlush(){
    fflush(stdout);
}

LogStream::LogStream()
{
    mOutput = defaultOutput;
    mFlush = defaultFlush;
}

LogStream::~LogStream()
{

    mBuffer.append('\n');
    mOutput(mBuffer.readPtr(), mBuffer.used());
}

template<typename T>
size_t convert(char buf[], T value);
size_t convertHex(char buf[], uintptr_t value);

template<typename T>
void LogStream::formatInteger(T v)
{
    if(mBuffer.avail() >= kMaxNumericSize){
        size_t len=convert(mBuffer.writePtr(),v);
        mBuffer.add(len);
    }
}

LogStream& LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

//将p指向的地址中的内容转换为16进制，void*指针可以接收任意类型的变量
LogStream& LogStream::operator<<(const void* p)
{
    //uintptr_t为本机uint指针的大小
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    if (mBuffer.avail() >= kMaxNumericSize)
    {
        char* buf = mBuffer.writePtr();
        buf[0] = '0';
        buf[1] = 'x';
        size_t len = convertHex(buf+2, v);
        mBuffer.add(len+2);
    }
    return *this;
}

// FIXME: replace this with Grisu3 by Florian Loitsch.
LogStream& LogStream::operator<<(double v)
{
    if (mBuffer.avail() >= kMaxNumericSize)
    {
        int len = snprintf(mBuffer.writePtr(), kMaxNumericSize, "%.12g", v);
        mBuffer.add(len);
    }
    return *this;
}

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
static_assert(sizeof(digits) == 20, "wrong number of digits");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

// Efficient Integer to String Conversions, by Matthew Wilson.
template<typename T>
size_t convert(char buf[], T value)
{
    T i = value;
    char* p = buf;

    do
    {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
    uintptr_t i = value;
    char* p = buf;

    do
    {
        int lsd = static_cast<int>(i % 16);
        i /= 16;
        *p++ = digitsHex[lsd];
    } while (i != 0);

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}






