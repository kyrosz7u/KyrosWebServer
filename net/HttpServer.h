//
// Created by 樱吹雪 on 2022/4/13.
//

#ifndef KYROSWEBSERVER_HTTPSERVER_H
#define KYROSWEBSERVER_HTTPSERVER_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Server.h"
#include <string>
#include <map>

using namespace std;

namespace net {
namespace http {
class HttpServer {
    typedef std::function<void(const HttpRequest *req, HttpResponse &resp)> HttpServeFunc;
public:
    //HttpServer默认起4个工作线程
    HttpServer(int port, int work_nums=4);
    void ListenAndServe(HttpServeFunc f);
private:
    void connectedCb(ConnPtr &conn);
    void readCb(ConnPtr &conn);
    void WriteCb(ConnPtr &conn);
    HttpServeFunc HttpCallback;
    Server mServer;
    const int mPort;
};

}//namespace http
}//namespace net

#endif //KYROSWEBSERVER_HTTPSERVER_H
