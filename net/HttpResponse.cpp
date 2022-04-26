//
// Created by 樱吹雪 on 2022/4/24.
//

#include "HttpResponse.h"
#include <cstdio>
#include <string>

using namespace net::http;

void HttpResponse::appendToBuffer(FixedBuffer &Buffer) {
    char buf[128];
    /* 注意：snprintf()函数的返回值是“要写入的字符串长度(不包括字符串结尾的'\0')，”
     * 不管buf是否能装下这个字符串。*/
    snprintf(buf, sizeof buf, "HTTP/1.1 %d %s\r\n", mStatusCode, kStatusMessage.at(mStatusCode));
    Buffer.append(buf);

    if(mAlive){
        Buffer.append("Connection: keep-alive\r\n");
    }else{
        Buffer.append("Connection: closed\r\n");
    }

    snprintf(buf, sizeof buf, "%s: %d\r\n", "Content-Length", mContentLength);
    Buffer.append(buf);

    for(auto _pair:mHeaders){
        snprintf(buf, sizeof buf, "%s: %s\r\n", _pair.first.c_str(), _pair.second.c_str());
        Buffer.append(buf);
    }
    Buffer.append("\r\n");
    if(mBody!=""){
        Buffer.append(mBody);
    }

}




