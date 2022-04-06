//
// Created by 樱吹雪 on 2022/4/2.
//

#ifndef KYROSWEBSERVER_SERVER_H
#define KYROSWEBSERVER_SERVER_H

#include "base/ThreadPool.h"
#include "base/Mutex.h"
#include "Connection.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <map>
#include <memory>
#include <functional>

#define MAX_EVENT_NUM 10000
#define LISTEN_BACKLOG 100

using namespace base;
using namespace std;

namespace net {
//处理网络连接读写的回调函数
typedef std::function<void(shared_ptr<Connection>&)> ConnCallBackFunc;

class Server {
public:
    Server(int port, int work_nums);
    ~Server();

    [[noreturn]] void EventLoop();
    void setReadCallBack(ConnCallBackFunc &&fb);
    void setWriteCallBack(ConnCallBackFunc &&fb);
private:
    void handleRead(int connfd);
    void handleWrite(int connfd);
    void handleErr(int connfd);
    void CloseConn(int connfd);

    int m_port;
    int m_listenfd;
    int m_epollfd;
    ConnCallBackFunc rCallBack;
    ConnCallBackFunc wCallBack;
    epoll_event m_event[MAX_EVENT_NUM];
    map<int,shared_ptr<Connection>> connMap;
    Mutex map_lock;
    ThreadPool *m_threadpool;

};

void epoll_add(int epfd, int fd);
void epoll_mod(int epfd, int fd, int ev);

}

#endif //KYROSWEBSERVER_SERVER_H
