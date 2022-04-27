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
    /* 注意： c++中指向临时生成的匿名对象需要用const指针，
     * 这里如果去掉const，那么std::bind生成的匿名function对象将无法被引用 */
    void setConnectedCallback(const ConnCallBackFunc &fb){ cCallBack=fb;}
    void setReadCallBack(const ConnCallBackFunc &fb){rCallBack=fb;}
    void setWriteCallBack(const ConnCallBackFunc &fb){wCallBack=fb;}
    void CloseConn(ConnPtr &conn);
private:
    void handleRead(ConnPtr &conn);
    void handleWrite(ConnPtr &conn);
    void handleErr(ConnPtr &conn);

    int mPort;
    int mListenFd;
    int mEpollFd;
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

}

#endif //KYROSWEBSERVER_SERVER_H
