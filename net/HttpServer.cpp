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
    mServer.setConnectedCallback(
            std::bind(&HttpServer::connectedCb, this, std::placeholders::_1));
    mServer.setReadCallBack(
            std::bind(&HttpServer::readCb, this, std::placeholders::_1));
    mServer.setWriteCallBack(
            std::bind(&HttpServer::WriteCb, this, std::placeholders::_1));
}

void HttpServer::connectedCb(ConnPtr &conn){
    conn->setContext(HttpContext());
    LOG_TRACE << "TcpServer::newConnection "<< conn->getFd();
};

void HttpServer::readCb(ConnPtr &conn){
    /* 为什么是any_cast<HttpContext> */
    HttpContext *ctx=boost::any_cast<HttpContext>(conn->getMutableContext());
    HttpRequest *req = ctx->getRequest();
    HttpResponse &resp = ctx->getResponse();

    HTTP_CODE ret= req->processRead(conn->mReadBuffer);
    if(ret==NO_REQUEST){
        LOG_TRACE << "NO_REQUEST" << conn->getFd();
        conn->enableConnRead();
        return ;
    }
    else if(ret==BAD_REQUEST){
        LOG_TRACE << "BAD_REQUEST" << conn->getFd();
        resp.setStatusCode(HttpResponse::CLIENT_ERROR_BAD_REQUEST);
        conn->mAlive= false;
        resp.appendToBuffer(conn->mWriteBuffer);
    }else if(ret==GET_REQUEST) {
        LOG_TRACE << "GET_REQUEST" << conn->getFd();
        resp.setStatusCode(HttpResponse::SUCCESS_OK);
        // http回调函数
        HttpCallback(req, resp);
        resp.appendToBuffer(conn->mWriteBuffer);
        // 设置要传输的文件
        if(resp.hasFile()){
            conn->setFile(resp.getFile());
        }
        conn->mAlive = resp.isAlive & req->isAlive;
        req->Init();
    }
    conn->enableConnWrite();
}
//
void HttpServer::WriteCb(ConnPtr &conn){
    if (!conn->mAlive) {
        mServer.CloseConn(conn);
    }
}

void HttpServer::ListenAndServe(HttpServeFunc f){
    HttpCallback = f;
    mServer.EventLoop();
}













