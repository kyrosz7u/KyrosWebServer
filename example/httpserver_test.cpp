//
// Created by 樱吹雪 on 2022/4/27.
//

#include "net/HttpServer.h"
#include "base/FileSystem.h"

using namespace net::http;
using namespace base;

void ServerHttp(const HttpRequest *req, HttpResponse &resp);

int main(){
    HttpServer s(2333);

    s.ListenAndServe(ServerHttp);

    return 0;
}

void ServerHttp(const HttpRequest *req, HttpResponse &resp){
    if(req->url=="/133"){
        resp.openFile("/home/xy/ClionDeploy/root/log.html");
        resp.setContentType("text/html");
        resp.isAlive = false;
    }

}

