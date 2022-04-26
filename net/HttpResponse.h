//
// Created by 樱吹雪 on 2022/4/24.
//

#ifndef KYROSWEBSERVER_HTTPRESPONSE_H
#define KYROSWEBSERVER_HTTPRESPONSE_H

#include "base/Buffer.h"
#include "base/StringPiece.h"
#include "base/Copyable.h"
#include <map>
#include <string>

using namespace base;
using namespace std;

namespace net {
namespace http {
class HttpResponse: public copyable {
public:
    enum STATUS_CODE{
        SUCCESS_OK=200,
        CLIENT_ERROR_BAD_REQUEST=400,
        CLIENT_ERROR_FORBIDDEN=403,
        CLIENT_ERROR_NOT_FOUND=404,
        SERVER_ERROR_INTERNAL_ERR=500,
    };
    const map<STATUS_CODE,StringPiece> kStatusMessage={
        {SUCCESS_OK,"OK"},
        {CLIENT_ERROR_BAD_REQUEST,"Your request has bad syntax "
                                "or is inherently impossible to staisfy.\n"},
        {CLIENT_ERROR_FORBIDDEN,"You do not have permission to "
                                "get file form this server.\n"},
        {CLIENT_ERROR_NOT_FOUND,"The requested file was not found on this server.\n"},
        {SERVER_ERROR_INTERNAL_ERR,"There was an unusual problem serving the request file.\n"},
    };
public:

    HttpResponse()
    {
        mStatusCode=CLIENT_ERROR_NOT_FOUND;
        mAlive= false;
        mContentLength=0;
    }
    void setStatusCode(STATUS_CODE StatusCode){ mStatusCode = StatusCode; }
    void setHeader(const string& key, const string& Val){ mHeaders[key] = Val; }
    void setContentLength(const int &len){mContentLength=len;}
    void setContentType(const string& ContentType){ mHeaders["Content-Type"] = ContentType; }
    void setConnection(bool isAlive){mAlive=isAlive;}
    void setBody(const string& Body){mBody=Body;}

    bool isClose(){ return !mAlive; }
    void appendToBuffer(FixedBuffer &Buffer);

private:
    STATUS_CODE mStatusCode;
    std::map<string, string> mHeaders;
    bool mAlive;
    int mContentLength;
    string mBody;
};

}//namespace http
}//namespace net

#endif //KYROSWEBSERVER_HTTPRESPONSE_H
