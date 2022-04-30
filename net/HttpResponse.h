//
// Created by 樱吹雪 on 2022/4/24.
//

#ifndef KYROSWEBSERVER_HTTPRESPONSE_H
#define KYROSWEBSERVER_HTTPRESPONSE_H

#include "base/Copyable.h"
#include "base/StringPiece.h"
#include "base/Buffer.h"
#include "base/FileSystem.h"
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
                                "or is inherently impossible to staisfy."},
        {CLIENT_ERROR_FORBIDDEN,"You do not have permission to "
                                "get file form this server."},
        {CLIENT_ERROR_NOT_FOUND,"The requested file was not found on this server."},
        {SERVER_ERROR_INTERNAL_ERR,"There was an unusual problem serving the request file."},
    };
public:

    HttpResponse();
    HttpResponse(STATUS_CODE StateCode);
    void setStatusCode(STATUS_CODE StatusCode){ mStatusCode = StatusCode; }
    void setHeader(const string& key, const string& Val){ Headers[key] = Val; }
    void setContentLength(const int &len){ ContentLength=len;}
    void setContentType(const string& ContentType){ Headers["Content-Type"] = ContentType; }
    void setBody(const string& ResponseBody){ Body=ResponseBody;}
    FileSystem::FILE_STATE openFile(const StringPiece &path);
    bool hasFile(){ return !(mFile.getAddr() == nullptr); }
    FileSystem& getFile(){ return mFile; }
    void appendToBuffer(FixedBuffer &Buffer);

public:
    std::map<string, string> Headers;
    bool isAlive;
    int ContentLength;
    string Body;
private:
    STATUS_CODE mStatusCode;
    FileSystem mFile;
};

}//namespace http
}//namespace net

#endif //KYROSWEBSERVER_HTTPRESPONSE_H
