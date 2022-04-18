//
// Created by 樱吹雪 on 2022/4/13.
//

#ifndef KYROSWEBSERVER_HTTPSERVER_H
#define KYROSWEBSERVER_HTTPSERVER_H

#include "net/Server.h"
#include <string>

using namespace std;

namespace net {
class HttpServer {
    /*http请求方法*/
    enum METHOD{
        GET, POST, HEAD, PUT, DELETE,
        TRACE, OPTIONS, CONNECT, PATH
    };
    /*http操作结果*/
    enum HTTP_CODE{
        NO_REQUEST,  // 请求未读完
        GET_REQUEST, // 获得请求
        BAD_REQUEST, // 错误请求
        NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST,
        INTERNAL_ERROR, CLOSED_CONNECTION,
    };
    /*http从状态机状态定义，用来读取每个行*/
    enum LINE_STATE{
        LINE_OK,    // 处理完http报文中一个完整的行
        LINE_OPEN,  // 未处理完http一个完整行，等待接收新的数据再处理
        LINE_BAD,   // 接收到的数据出错了
    };
    /*http主状态机状态定义，用来控制分析报文*/
    enum CHECK_STATE {
        CHECK_STATE_REQUEST,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT,
    };
    /*http请求结构体*/
    struct httpRequest{
        string url;
        string version;
        string host;
        bool linger;
        METHOD method;
        long int contentLength;
    };

    /*http响应结构体*/
    struct httpResponse{

    };

    struct httpConnection{
        httpRequest Request;
        httpResponse Response;
        unsigned int mCheckPtr;
        CHECK_STATE mCheckState;
    };
public:
    //HttpServer默认起4个工作线程
    HttpServer(int port)
            : mPort(port),
              mServer(port,4){}
    HttpServer(int port, int work_nums)
            : mPort(port),
            mServer(port,work_nums){}
    void Listen(){mServer.EventLoop()};
    void HandleReadandWrite();
    LINE_STATE parse_line(char* text);
    HTTP_CODE parse_request(char* text);
    HTTP_CODE parse_header(char* text);
private:
    Server mServer;
    const int mPort;

};


}//namespace net

#endif //KYROSWEBSERVER_HTTPSERVER_H
