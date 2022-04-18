//
// Created by 樱吹雪 on 2022/4/4.
//

#include "Connection.h"
#include <sys/socket.h>
#include <string.h>

namespace net {

/*读取connection的buffer，并返回读取长度*/
int Connection::Read(char* buf,size_t len){
    int ret;
    if(len<0)
        return -1;
    if(len<mReadBuffer.used()){
        memcpy(buf,mReadBuffer.readPtr(),len);
        ret=len;
        mReadBuffer.readPtrMove(len);
    }else{
        memcpy(buf,mReadBuffer.readPtr(),mReadBuffer.used());
        ret=mReadBuffer.used();
        mReadBuffer.reset();
    }
    return ret;

}

int Connection::Write(char* buf,size_t len){
    mWriteBuffer.append(buf, len);
}

/*读取收到的数据包，并放入mReadBuffer*/
int Connection::recv() {
    return mReadBuffer.readFd(mConnfd);
}

/*将mWriteBuffer中的数据发送出去*/
int Connection::send() {
    int write_bytes;
    int write_sum=0;

    while(mWriteBuffer.used()>0){
        write_bytes = ::send(mConnfd, mWriteBuffer.readPtr(), mWriteBuffer.used(), 0);
        if(write_bytes<0){
            break;
        }
        mWriteBuffer.readPtrMove(write_bytes);
        write_sum+=write_bytes;
    }

    if(write_bytes<0) {
        if (errno != EAGAIN) {
            return -1;
        }
    }
    return write_sum;
}

}//namespace net