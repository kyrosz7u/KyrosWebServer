//
// Created by 樱吹雪 on 2022/4/4.
//

#include "Connection.h"
#include "base/Logger.h"
#include <sys/socket.h>
#include <string.h>

using namespace net;

/*读取connection的buffer，并返回读取长度*/
int Connection::Read(char* buf,size_t len){
    int ret;
    if(len<0)
        return -1;
    if(len<mReadBuffer.used()){
        memcpy(buf,mReadBuffer.readPtr(),len);
        ret=len;
        mReadBuffer.readPtrMove(len);
    }else{
        memcpy(buf,mReadBuffer.readPtr(),mReadBuffer.used());
        ret=mReadBuffer.used();
        mReadBuffer.reset();
    }
    return ret;

}

int Connection::Write(char* buf,size_t len){
    mWriteBuffer.append(buf, len);
}

/*读取收到的数据包，并放入mReadBuffer*/
int Connection::recv() {
    return mReadBuffer.readFd(mConnfd);
}

/* 将mWriteBuffer中的数据发送出去*/
/* 返回 -1：出错  0：发送完毕   >0：待发送数量*/
long int Connection::send() {
    int write_bytes;
    int write_sum=0;

    while(1){
        m_iv[0].iov_base=(char*)mWriteBuffer.readPtr();
        m_iv[0].iov_len = mWriteBuffer.used();
        write_bytes = ::writev(mConnfd, m_iv, 2);
        LOG_TRACE << write_bytes;
        if(write_bytes<=0){
            break;
        }
        if(write_bytes>mWriteBuffer.used()){
            size_t temp = write_bytes - mWriteBuffer.used();
            m_iv[1].iov_base = (char*)(m_iv[1].iov_base) + temp;
            m_iv[1].iov_len -= temp;
            mWriteBuffer.reset();
        }else{
            mWriteBuffer.readPtrMove(write_bytes);
        }
        write_sum+=write_bytes;
    }

    if(write_bytes==-1){
        if (errno == EAGAIN) {
            return write_bytes=mWriteBuffer.used() + m_iv[1].iov_len;
        }
        LOG_TRACE << "errno"<<sys_errlist[errno];
    }
    m_iv[1].iov_base= nullptr;
    m_iv[1].iov_len= 0;
    return write_bytes;
}

int Connection::getFd() {
    return mConnfd;
}

void Connection::epoll_mod(int epfd, int fd, int ev)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
}
