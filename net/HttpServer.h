//
// Created by 樱吹雪 on 2022/4/13.
//

#ifndef KYROSWEBSERVER_HTTPSERVER_H
#define KYROSWEBSERVER_HTTPSERVER_H

#include "net/Server.h"
#include <string>
#include <map>

using namespace std;

namespace net {
class HttpServer {

public:
    //HttpServer默认起4个工作线程
    HttpServer(int port)
            : mPort(port),
              mServer(port,4){}
    HttpServer(int port, int work_nums)
            : mPort(port),
            mServer(port,work_nums){}
    void Listen();
    void HandleReadandWrite();
private:
    void connectedCb(ConnPtr &conn);
    Server mServer;
    const int mPort;
};


}//namespace net

#endif //KYROSWEBSERVER_HTTPSERVER_H
