//
// Created by 樱吹雪 on 2022/4/19.
//

#include "HttpContext.h"
#include "base/Logger.h"
#include <cstring>

namespace net{
namespace http{

bool HttpContext::parseRequest(FixedBuffer& Buffer){
    HTTP_CODE ret = Request.processRead(Buffer);
    if(ret==GET_REQUEST||ret==NO_REQUEST){
        return true;
    }else{
        return false;
    }
}

}//namespace http
}//namespace net

