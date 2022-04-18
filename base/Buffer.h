//
// Created by 樱吹雪 on 2022/4/15.
//

#ifndef KYROSWEBSERVER_FIXEDBUFFER_H
#define KYROSWEBSERVER_FIXEDBUFFER_H

#include "Copyable.h"
#include <stdint.h>
#include <cstring>
#include <string>

namespace base {
class FixedBuffer : public noncopyable{
public:
    const int kBufferSize = 1024;
    FixedBuffer()
    {
        _buf = new char[kBufferSize];
        _end = _buf + kBufferSize;
        mReadPtr=_buf;
        mWritePtr=_buf;
    }
    //深拷贝buffer
    FixedBuffer(FixedBuffer& b)
    {
        size_t len=b.length();
        _buf = new char[len];
        _end = _buf + len;
        mReadPtr = _buf + static_cast<size_t>(b.mReadPtr - b._buf);
        mWritePtr = _buf + static_cast<size_t>(b.mWritePtr - b._buf);
        memcpy(mReadPtr, b.mReadPtr, b.used());
    }
    FixedBuffer& operator=(FixedBuffer& b)
    {
        size_t len=b.length();
        _buf = new char[len];
        _end = _buf + len;
        mReadPtr = _buf + static_cast<size_t>(b.mReadPtr - b._buf);
        mWritePtr = _buf + static_cast<size_t>(b.mWritePtr - b._buf);
        memcpy(mReadPtr, b.mReadPtr, b.used());
        return *this;
    }
    ~FixedBuffer() {
        delete _buf;
        _buf = nullptr;
    }
    int append(const char &ch);
    int append(const char *str, size_t len);
    int append(const char *str);
    int append(std::string &str);
    int readFd(int fd);

    inline const char *readPtr() { return mReadPtr;}
    inline const char *start() { return _buf; }

    int readPtrMove(int len){
        char *temp= mReadPtr+=len;
        if(temp>=_buf&& temp <= mWritePtr){
            mReadPtr=temp;
            return 0;
        }else{
            return -1;
        }
    }

    inline char* writePtr(){return mWritePtr;}

    int add(size_t len) {
        if (len > avail())
            return -1;
        else {
            mWritePtr += len;
            return len;
        }
    }

    // 返回buffer已使用的空间
    inline size_t used() { return static_cast<size_t>(mWritePtr - mReadPtr); }
    // 返回buffer的大小
    inline size_t length() { return static_cast<size_t>(_end - _buf); }
    // 返回buffer可用的空间
    inline size_t avail() { return static_cast<size_t>(_end - _buf) -
                    static_cast<size_t>(mWritePtr - mReadPtr); }

    // 给buffer清零
    void clear() {
        memset(_buf, 0, _end - _buf);
        reset();
    }
    // 重置读写指针
    void reset() { mWritePtr = _buf; mReadPtr= _buf;}

private:
    //_buf<=mReadPtr<=mWritePtr<=_end
    char *mReadPtr;
    char *_buf;
    char *_end;
    char *mWritePtr;

};
}//namespace base

#endif //KYROSWEBSERVER_FIXEDBUFFER_H
