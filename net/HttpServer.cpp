//
// Created by 樱吹雪 on 2022/4/13.
//

#include "HttpServer.h"
#include "HttpContext.h"
#include "Connection.h"
#include "base/Logger.h"
#include <cstring>
#include <functional>

using namespace net::http;

HttpServer::HttpServer(int port, int work_nums)
        : mPort(port),
          mServer(port,work_nums)
{
    mServer.setConnectedCallBack(
            std::bind(&HttpServer::connectedCb, this, std::placeholders::_1));
    mServer.setReadCallBack(
            std::bind(&HttpServer::readCb, this, std::placeholders::_1));
}

void HttpServer::connectedCb(ConnPtr &conn){
    conn->setContext(HttpContext());
    LOG_INFO << "TcpServer::newConnection "<< conn->getFd();
};

void HttpServer::readCb(ConnPtr &conn){
    HttpContext &ctx=boost::any_cast<HttpContext&>(conn->getContext());
    bool ret= ctx.parseRequest(conn->mReadBuffer);
    if(!ret){
        mServer.CloseConn(conn);
    }
    if(ctx.getRequestState()==HttpRequest::CHECK_STATE_SUCCESS){
        const HttpRequest &req = ctx.getRequest();
        HttpResponse resp;
        HttpCallback(req,resp);
        resp.appendToBuffer(conn->mWriteBuffer);
        if(resp.isClose()){
            mServer.CloseConn(conn);
        }
        ctx.Init();
    }
}

void HttpServer::ListenAndServe(HttpServeFunc f){
    HttpCallback = f;
    mServer.EventLoop();
}













