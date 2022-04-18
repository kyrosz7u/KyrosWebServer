//
// Created by 樱吹雪 on 2022/4/2.
//

#include "Server.h"
#include "Connection.h"
#include "base/Logger.h"
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include<iostream>

namespace net{

Server::Server(int port, int work_nums){
    mPort = port;
    mListenfd=socket(PF_INET, SOCK_STREAM, 0);

    //表示socket网络地址的结构体，需要设置好协议、地址和端口
    sockaddr_in addr;
    bzero(&addr,sizeof addr);
    addr.sin_family=AF_INET;

    //INADDR_ANY设置为本机任意网卡的地址
    //htons：uint16_t主机端转网络端    htonl：uint32_t主机端转网络端
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(port);

    int ret=bind(mListenfd, (sockaddr*)&addr, sizeof addr);
    assert(ret==0);
    mEpollfd=epoll_create(5);

    //LISTEN_BACKLOG设置内核监听队列的最大长度。
    //当监听长度大于LISTEN_BACKLOG时，服务器将不再受理新的客户连接，
    //客户端将收到ECONNREFUSED错误信息。
    listen(mListenfd, LISTEN_BACKLOG);

    LOG_INFO<<"Port"<<port<<"Listening...";

    epoll_event ev;
    ev.data.fd=mListenfd;
    ev.events=EPOLLIN|EPOLLRDHUP;
    epoll_ctl(mEpollfd, EPOLL_CTL_ADD, mListenfd, &ev);

    mThreadpool = new ThreadPool(work_nums);
    mThreadpool->Start();

}

Server::~Server(){
    mThreadpool->Stop();
    close(mListenfd);
    close(mEpollfd);
    delete mThreadpool;
}

void Server::setReadCallBack(ConnCallBackFunc &&fb){
    rCallBack=std::move(fb);
}

void Server::setWriteCallBack(ConnCallBackFunc &&fb){
    wCallBack=std::move(fb);
}

void Server::handleRead(int connfd) {
    LOG_DEBUG<<"connfd:"<<connfd<<"handleRead";
    int ret = connMap[connfd]->recv();
    if(ret<=0){
        CloseConn(connfd);
        return ;
    }
    LOG_DEBUG<<"recv:"<<connMap[connfd]->mReadBuffer.readPtr();
    if(rCallBack){
        rCallBack(connMap[connfd]);
    }
    epoll_mod(mEpollfd, connfd, EPOLLOUT);
}

void Server::handleWrite(int connfd) {
    LOG_DEBUG<<"connfd:"<<connfd<<"handleWrite";
    int ret = connMap[connfd]->send();
    if(ret==-1){
        CloseConn(connfd);
        return ;
    }
    LOG_DEBUG<<"send:"<<connMap[connfd]->mWriteBuffer.readPtr();
    if(wCallBack){
        wCallBack(connMap[connfd]);
    }
    epoll_mod(mEpollfd, connfd, EPOLLIN);
}

void Server::handleErr(int connfd) {

}

//map默认不是线程安全的
void Server::CloseConn(int connfd) {
    shutdown(connfd,SHUT_RD);
    mapLock.Lock();
    epoll_ctl(mEpollfd, EPOLL_CTL_DEL, connfd, 0);
    connMap.erase(connfd);
    mapLock.Unlock();
    LOG_TRACE<<"connfd:"<<connfd<<"Closed";
}

[[noreturn]] void Server::EventLoop() {
    int ep_nums;
    while(1){
        //-1表示一直阻塞到有事件到达
        ep_nums=epoll_wait(mEpollfd, epollEvent, MAX_EVENT_NUM, -1);
        LOG_TRACE<<"epoll_wakeup";
        for(int i=0;i<ep_nums;i++){
            if(epollEvent[i].data.fd == mListenfd){
                sockaddr_in client_addr;
                socklen_t addr_len;
                addr_len = sizeof client_addr;
                int connfd=accept(mListenfd, (sockaddr*)&client_addr, &addr_len);
                epoll_add(mEpollfd, connfd);
                mapLock.Lock();
                connMap[connfd] = std::move(shared_ptr<Connection>(new Connection(connfd)));
                mapLock.Unlock();
            //出错或者连接被远程关闭，直接释放本地的Connection对象
            }else if (epollEvent[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                CloseConn(epollEvent[i].data.fd);
            }
            else{
                int connfd = epollEvent[i].data.fd;
                if(epollEvent[i].events & EPOLLIN){
                    LOG_TRACE<<"epoll connfd:"<<connfd<<"Read";
                    mThreadpool->AddTask(std::bind(&Server::handleRead, this, connfd));
                }else if(epollEvent[i].events & EPOLLOUT){
                    LOG_TRACE<<"epoll connfd:"<<connfd<<"Write";
                    mThreadpool->AddTask(std::bind(&Server::handleWrite, this, connfd));
                }else if(epollEvent[i].events & EPOLLERR){
                    LOG_TRACE<<"epoll connfd:"<<connfd<<"Err";
                    mThreadpool->AddTask(std::bind(&Server::handleErr, this, connfd));
                }else{}
            }
        }
    }
}

//将文件描述符设置为非阻塞
int setnonblocking(int fd){
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;

    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void epoll_add(int epfd, int fd){
    epoll_event ev;
    ev.data.fd=fd;
    //EPOLLRDHUP在对端关闭的时候会触发
    ev.events=EPOLLIN|EPOLLONESHOT|EPOLLET|EPOLLRDHUP;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    //1、对于监听的sockfd，最好使用水平触发模式，边缘触发模式会导致高并发情况下，有的客户端会连接不上。
    // 如果非要使用边缘触发，网上有的方案是用while来循环accept()。
    //2、对于读写的connfd，水平触发模式下，阻塞和非阻塞效果都一样，不过为了防止特殊情况，还是建议设置非阻塞。
    //3、对于读写的connfd，边缘触发模式下，必须使用非阻塞IO，并要一次性全部读写完数据。
    //https://blog.csdn.net/Jiangtagong/article/details/116356621
    setnonblocking(fd);
}

//将事件重置为EPOLLONESHOT
void epoll_mod(int epfd, int fd, int ev)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;

    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
}

}//namespace net

