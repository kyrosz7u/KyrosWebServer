//
// Created by 樱吹雪 on 2022/4/4.
//

#ifndef KYROSWEBSERVER_CONNECTION_H
#define KYROSWEBSERVER_CONNECTION_H

#include "base/Buffer.h"
#include "base/FileSystem.h"
#include "boost/any.hpp"
#include <sys/uio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <functional>
#include <memory>

using namespace std;
using namespace base;

namespace net {
//系统为每个连接分配固定大小的读写缓冲区
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024

class Connection;

// 定义指向连接的智能指针
typedef shared_ptr<Connection> ConnPtr;
class Connection {
public:
    Connection(int socketfd, int epfd)
    : mConnfd(socketfd),
      mPoller(epfd){
        mAlive = true;
        m_iv[0] = {nullptr, 0};
        m_iv[1] = {nullptr, 0};
    }
    ~Connection(){close(mConnfd);}
    int Read(char* buf,size_t len);
    int Write(char* buf,size_t len);
    int recv();
    long int send();
    int getFd();
    boost::any& getContext(){ return mContext; }
    boost::any* getMutableContext(){return &mContext;}
    void setContext(const boost::any &c){ mContext = c; }
    void setFile(FileSystem &F){
        m_iv[1].iov_base= F.getAddr();
        m_iv[1].iov_len = F.getLength();
    }
    void enableConnRead(){epoll_mod(mPoller, mConnfd, EPOLLIN);}
    void enableConnWrite(){epoll_mod(mPoller, mConnfd, EPOLLOUT);}
    static void epoll_mod(int epfd, int fd, int ev);
    FixedBuffer mReadBuffer;
    FixedBuffer mWriteBuffer;

public:
    bool mAlive;
private:
    int mConnfd;
    int mPoller;
    boost::any mContext;
    struct iovec m_iv[2];
};


}//namespace net

#endif //KYROSWEBSERVER_CONNECTION_H
