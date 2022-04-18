//
// Created by 樱吹雪 on 2022/4/13.
//

#include "HttpServer.h"
#include "Connection.h"
#include "base/Logger.h"
#include <cstring>

using namespace net;

HttpServer::LINE_STATE HttpServer::parse_line(char* rd_buf, size_t rb_ptr){
    for (; mCheckPtr < rb_ptr; mCheckPtr++) {
        char ch = rd_buf[mCheckPtr];
        if(ch=='\r'){
            if(mCheckPtr+1==rb_ptr){
                return LINE_OPEN;
            }else if(rd_buf[mCheckPtr+1]=='\n'){
                rd_buf[mCheckPtr] = '\0';
                rd_buf[mCheckPtr+1] = '\0';
                mCheckPtr+=2;
                return LINE_OK;
            }else{
                return LINE_BAD;
            }
        }else if(ch=='\n'){
            //必须接收到2个以上字符且上一个字符是\r
            if(mCheckPtr>1&&rd_buf[mCheckPtr-1]=='\r'){
                rd_buf[mCheckPtr] = '\0';
                rd_buf[mCheckPtr-1] = '\0';
                mCheckPtr++;
                return LINE_OK;
            }else{
                return LINE_BAD;
            }
        }
    }
    return LINE_OPEN;
}

//在HTTP报文中，请求行用来说明请求类型
//要访问的资源以及所使用的HTTP版本，其中各个部分之间通过\t或空格分隔。
HttpServer::HTTP_CODE HttpServer::parse_request(char* text)
{
    //示例：text ="GET /index.html HTTP/1.1\r\n"
//    char* text=rb_buf+mCheckStart;

    //strpbrk在源字符串（s1）中找出最先含有搜索字符串（s2）
    //中任一(子)字符的位置并返回，若找不到则返回空指针
    char *url=strpbrk(text," \t");    //字符匹配
    if(!url){
        return BAD_REQUEST;
    }
    *url++='\0';
    char *method=text;  //method="GET"

    //strcasecmp判断str1和str2是否相同
    // 并且忽略字母大小写差异  等于0表示相同 大于0表示str1长度大于str2
    if(strcasecmp(method,"GET")==0){
        mRequest.method = GET;
    }else{
        return BAD_REQUEST;
    }
    // 此时url跳过了第一个空格或者'\t'，
    // 但http请求行中url结束位置还有空格或者'\t'
    // 需要继续跳过

    // strspn()函数用来计算字符串str中连续有几个字符都属于字符串 accept
    // 如果str所包含的字符都属于accept，返回accept长度，如果第一个字符不属于返回0
    // 即在这里用来跳过含有url中开头含有" \t"的前部分
    url+=strspn(url, " \t");
    char *version = strpbrk(url, " \t");
    if(!version)
        return BAD_REQUEST;
    *version++ = '\0';  // url="/index.html"

    version+=strspn(version, " \t");

    if(strcasecmp(version,"HTTP/1.1")!=0){
        return BAD_REQUEST;
    }

    // 对请求资源的前七个字符判断，去掉有些报文中的http://
    if(strncasecmp(url,"http://",7)==0){
        url += 7;
        url = strchr(url, '/');   //find the first '/' in url
    }

    if(!url||url[0]!='/'){
        return BAD_REQUEST;
    }

    mCheckState=CHECK_STATE_HEADER;
    return NO_REQUEST;
}

HttpServer::HTTP_CODE HttpServer::parse_header(char* text)
{
    //空请求头，表示header结束
    if(text[0]=='\0'){
        if (mRequest.contentLength != 0) {
            // 有消息体需要处理 POST方法
            mCheckState=CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }
    else if(strncasecmp(text,"Connection:",11)==0){
        text+=11;
        text+=strspn(text," \t");
        if(strcasecmp(text,"keep-alive")==0){
            mRequest.linger= true;
        }else{
            mRequest.linger= false;
        }
        return NO_REQUEST;
    }
    else if(strncasecmp(text, "Content-length:", 15)==0){
        text+=15;
        text+=strspn(text," \t");
        mRequest.contentLength = atol(text);
        return NO_REQUEST;
    }
    else if(strncasecmp(text, "Host:", 5)==0)
    {
        text+=5;
        text += strspn(text, " \t");
        mRequest.host = text;
        return NO_REQUEST;
    }
    else{
        LOG_INFO<<"oop!unknow header: "<<text;
    }
    return NO_REQUEST;
}











