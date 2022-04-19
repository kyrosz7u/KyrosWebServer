//
// Created by 樱吹雪 on 2022/4/19.
//

#ifndef KYROSWEBSERVER_HTTPCONTEXT_H
#define KYROSWEBSERVER_HTTPCONTEXT_H


class HttpContext {
public:
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
        CHECK_STATE_SUCCESS,
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
    LINE_STATE parse_line(char* text);
    HTTP_CODE parse_request(char* text);
    HTTP_CODE parse_header(char* text);

private:

};


#endif //KYROSWEBSERVER_HTTPCONTEXT_H
