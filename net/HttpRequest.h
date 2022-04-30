//
// Created by 樱吹雪 on 2022/4/25.
//
#ifndef KYROSWEBSERVER_HTTPREQUEST_H
#define KYROSWEBSERVER_HTTPREQUEST_H

#include "base/Buffer.h"
#include <string>

using namespace std;
using namespace base;

namespace net{
namespace http{
/*http请求方法*/
enum METHOD {
    GET, POST, HEAD, PUT, DELETE,
    TRACE, OPTIONS, CONNECT, PATH
};
/*http操作结果*/
enum HTTP_CODE {
    NO_REQUEST,  // 请求未读完
    GET_REQUEST, // 获得请求
    BAD_REQUEST, // 错误请求
    NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST,
    INTERNAL_ERROR, CLOSED_CONNECTION,
};
class HttpRequest {
public:
    /*http从状态机状态定义，用来读取每个行*/
    enum LINE_STATE {
        LINE_OK,    // 处理完http报文中一个完整的行
        LINE_OPEN,  // 未处理完http一个完整行，等待接收新的数据再处理
        LINE_BAD,   // 接收到的数据出错了
    };
    /*http主状态机状态定义，用来控制分析报文*/
    enum CHECK_STATE {
        CHECK_STATE_REQUEST,
        CHECK_STATE_HEADER,
        CHECK_STATE_BODY,
        CHECK_STATE_SUCCESS,
    };

    HttpRequest()
    {
        mCheckState=CHECK_STATE_REQUEST;
        mCheckIndex=0;
        mLineState = LINE_OPEN;
    }
    void Init()
    {
        mCheckIndex=CHECK_STATE_REQUEST;
        mCheckIndex=0;
        mLineState = LINE_OPEN;
    }

    HTTP_CODE processRead(FixedBuffer &buf);
    CHECK_STATE getCheckState() { return mCheckState; }
    LINE_STATE getLineState() { return mLineState; }
    unsigned int getCheckIndex() { return mCheckIndex; }

public:
    string url;
    string version;
    string host;
    bool isAlive=false;
    METHOD method;
    long int contentLength=0;
    string body;
private:
    LINE_STATE parseLine(char* rd_buf, size_t sz);
    HTTP_CODE parseRequest(char *text);
    HTTP_CODE parseHeader(char *checkptr);
    HTTP_CODE parseContent(char *text);

    unsigned int mCheckIndex;
    CHECK_STATE mCheckState;
    LINE_STATE mLineState;
};

}//namespace http
}//namespace net

#endif
