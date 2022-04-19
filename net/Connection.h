//
// Created by 樱吹雪 on 2022/4/4.
//

#ifndef KYROSWEBSERVER_CONNECTION_H
#define KYROSWEBSERVER_CONNECTION_H

#include "base/Buffer.h"
#include "boost/any.hpp"
#include <unistd.h>
#include <functional>
#include <memory>

using namespace base;

namespace net {
//系统为每个连接分配固定大小的读写缓冲区
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024

class Connection {
public:
    Connection(int socketfd)
    : mConnfd(socketfd)
    {}
    ~Connection(){close(mConnfd);}
    int Read(char* buf,size_t len);
    int Write(char* buf,size_t len);
    int recv();
    int send();
    int getFd();
    boost::any& getContext(){ return mContext; }
    void setContext(const boost::any &c){ mContext = c; }
    FixedBuffer mReadBuffer;
    FixedBuffer mWriteBuffer;
private:
    int mConnfd;
    boost::any mContext;

};
}//namespace net

#endif //KYROSWEBSERVER_CONNECTION_H
