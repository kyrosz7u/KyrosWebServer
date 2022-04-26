//
// Created by 樱吹雪 on 2022/4/19.
//
#pragma once

#include "Connection.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "base/Buffer.h"

namespace net {
namespace http {

class HttpContext {
public:
    HttpContext(){

    }
    void Init()
    {
        Request.Init();
    }
    bool parseRequest(FixedBuffer& Buffer);
    HttpRequest::CHECK_STATE getRequestState() { return Request.getCheckState(); }
    const HttpRequest& getRequest() { return Request; }

private:
    HttpRequest Request;
};
}//namespace http
}//namespace net

