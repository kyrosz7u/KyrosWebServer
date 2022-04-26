//
// Created by 樱吹雪 on 2022/4/2.
//

#ifndef KYROSWEBSERVER_SERVER_H
#define KYROSWEBSERVER_SERVER_H

#include "base/Copyable.h"
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
// 定义处理网络连接读写的回调函数
typedef function<void(ConnPtr&)> ConnCallBackFunc;

//通用的服务器类，通过注册回调函数的方式处理连接的读写
class Server : public noncopyable{
public:
    Server(int port, int work_nums);
    ~Server();

    [[noreturn]] void EventLoop();
    void setConnectedCallBack(ConnCallBackFunc &&fb);
    void setReadCallBack(ConnCallBackFunc &&fb);
    void setWriteCallBack(ConnCallBackFunc &&fb);
    void CloseConn(ConnPtr &conn);
private:
    void handleRead(ConnPtr &conn);
    void handleWrite(ConnPtr &conn);
    void handleErr(ConnPtr &conn);

    int mPort;
    int mListenfd;
    int mEpollfd;
    // 只有主线程执行回调函数
    ConnCallBackFunc cCallBack;
    ConnCallBackFunc rCallBack;
    ConnCallBackFunc wCallBack;
    epoll_event epollEvent[MAX_EVENT_NUM];
    map<int,ConnPtr> connMap;
    Mutex mapLock;
    ThreadPool *mThreadpool;

};

void epoll_add(int epfd, int fd);
void epoll_mod(int epfd, int fd, int ev);

}

#endif //KYROSWEBSERVER_SERVER_H
