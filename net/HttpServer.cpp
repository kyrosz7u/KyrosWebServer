//
// Created by 樱吹雪 on 2022/4/13.
//

#include "HttpServer.h"
#include "Connection.h"
#include "base/Logger.h"
#include <cstring>

using namespace net;

void HttpServer::connectedCb(ConnPtr &conn){
    conn->setContext();
    LOG_INFO << "TcpServer::newConnection "<< conn->getFd();
};

void HttpServer::Listen(){
//    mServer.setConnectedCallBack();
    mServer.EventLoop();
}













