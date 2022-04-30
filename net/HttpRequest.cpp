//
// Created by 樱吹雪 on 2022/4/25.
//

#include "HttpRequest.h"
#include "base/Logger.h"

using namespace net::http;

HTTP_CODE HttpRequest::processRead(FixedBuffer &buf){
    HTTP_CODE ret=NO_REQUEST;
    while(buf.used()>0){
        char* readptr = const_cast<char*>(buf.readPtr());
        mLineState = parseLine(readptr, buf.used());
        if(mLineState == LINE_OPEN&&mCheckState!=CHECK_STATE_BODY){
            break;;
        }else if(mLineState == LINE_BAD){
            return BAD_REQUEST;
        }
        /*LINE OK*/
        switch(mCheckState){
            case CHECK_STATE_REQUEST:
            {
                ret= parseRequest(readptr);
                if (ret == BAD_REQUEST) {
                    return BAD_REQUEST;
                }
                buf.readPtrMove(mCheckIndex);
                mCheckIndex=0;
                break;
            }
            case CHECK_STATE_HEADER:
            {
                ret= parseHeader(readptr);
                if (ret == BAD_REQUEST){
                    return ret;
                }
                buf.readPtrMove(mCheckIndex);
                mCheckIndex=0;
                if(ret==GET_REQUEST){
                    return GET_REQUEST;
                }
                break;
            }
            case CHECK_STATE_BODY:
            {
                // 大于buffersize拒绝接收
                if(contentLength>1024){
                    LOG_WARN << "contentLength much than buffersize";
                }
                ret= parseContent(readptr);
                if (ret == BAD_REQUEST) {
                    return BAD_REQUEST;
                }else{
                    return ret;
                }
            }
        }
    }
    return NO_REQUEST;
}

HttpRequest::LINE_STATE HttpRequest::parseLine(char* rd_buf, size_t sz){
    for (; mCheckIndex < sz; mCheckIndex++) {
        char ch = rd_buf[mCheckIndex];
        if(ch=='\r'){
            if(mCheckIndex + 1 == sz){
                return LINE_OPEN;
            }else if(rd_buf[mCheckIndex+1] == '\n'){
                rd_buf[mCheckIndex] = '\0';
                rd_buf[mCheckIndex+1] = '\0';
                mCheckIndex+=2;
                return LINE_OK;
            }else{
                return LINE_BAD;
            }
        }else if(ch=='\n'){
            //必须接收到2个以上字符且上一个字符是\r
            if(mCheckIndex > 1 && rd_buf[mCheckIndex-1] == '\r'){
                rd_buf[mCheckIndex] = '\0';
                rd_buf[mCheckIndex-1] = '\0';
                mCheckIndex++;
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
HTTP_CODE HttpRequest::parseRequest(char* text)
{
    //示例：text ="GET /index.html HTTP/1.1\r\n"
//    char* text=rb_buf+mCheckStart;

    //strpbrk在源字符串（s1）中找出最先含有搜索字符串（s2）
    //中任一(子)字符的位置并返回，若找不到则返回空指针
    char *_url=strpbrk(text," \t");    //字符匹配
    if(!_url){
        return BAD_REQUEST;
    }
    *_url++='\0';
    char *_method=text;  //method="GET"

    //strcasecmp判断str1和str2是否相同
    // 并且忽略字母大小写差异  等于0表示相同 大于0表示str1长度大于str2
    if(strcasecmp(_method,"GET")==0){
        method = GET;
    }else if(strcasecmp(_method,"POST")==0){
        method = POST;
    }
    else{
        return BAD_REQUEST;
    }
    // 此时url跳过了第一个空格或者'\t'，
    // 但http请求行中url结束位置还有空格或者'\t'
    // 需要继续跳过

    // strspn()函数用来计算字符串str中连续有几个字符都属于字符串 accept
    // 如果str所包含的字符都属于accept，返回accept长度，如果第一个字符不属于返回0
    // 即在这里用来跳过含有url中开头含有" \t"的前部分
    _url+=strspn(_url, " \t");
    char *_version = strpbrk(_url, " \t");
    if(!_version)
        return BAD_REQUEST;

    *_version++ = '\0';  // url="/index.html"

    _version+=strspn(_version, " \t");

    if(strcasecmp(_version,"HTTP/1.1")!=0){
        return BAD_REQUEST;
    }

    // 对请求资源的前七个字符判断，去掉有些报文中的http://
    if(strncasecmp(_url,"http://",7)==0){
        url += 7;
        url = strchr(_url, '/');   //find the first '/' in url
    }

    if(!_url||_url[0]!='/'){
        return BAD_REQUEST;
    }
    url=_url;
    version=_version;
    mCheckState=CHECK_STATE_HEADER;
    return NO_REQUEST;
}

HTTP_CODE HttpRequest::parseHeader(char* text)
{
    char* checkptr=text;
    //空请求头，表示header结束
    if(checkptr[0] == '\0'){
        if (contentLength != 0) {
            // 有消息体需要处理 POST方法
            mCheckState=CHECK_STATE_BODY;
            return NO_REQUEST;
        }
        mCheckState = CHECK_STATE_SUCCESS;
        return GET_REQUEST;
    }
    else if(strncasecmp(checkptr, "Connection:", 11) == 0){
        checkptr+=11;
        checkptr+=strspn(checkptr, " \t");
        if(strcasecmp(checkptr, "keep-isAlive") == 0){
            isAlive= true;
        }else{
            isAlive= false;
        }
        return NO_REQUEST;
    }
    else if(strncasecmp(checkptr, "Content-length:", 15) == 0){
        checkptr+=15;
        checkptr+=strspn(checkptr, " \t");
        contentLength = atol(checkptr);
        return NO_REQUEST;
    }
    else if(strncasecmp(checkptr, "Host:", 5) == 0)
    {
        checkptr+=5;
        checkptr += strspn(checkptr, " \t");
        host = checkptr;
        return NO_REQUEST;
    }
    else{
        LOG_DEBUG << "oop!unknow header: " << checkptr;
    }
    return NO_REQUEST;
}


HTTP_CODE HttpRequest::parseContent(char* text)
{
    if(mCheckIndex>=contentLength){
        body.assign(text, contentLength);
        mCheckState = CHECK_STATE_SUCCESS;
        return GET_REQUEST;
    } else {
        return NO_REQUEST;
    }

}


