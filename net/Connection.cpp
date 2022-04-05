//
// Created by 樱吹雪 on 2022/4/4.
//

#include "Connection.h"
#include <sys/socket.h>
#include <string.h>

namespace net {

int Connection::Read(char* buf,int len){
    int ret;
    if(len<0)
        return -1;
    if(len<mReadPtr){
        memcpy(buf,m_rBuffer,len);
        ret=len;
        memcpy(m_rBuffer,m_rBuffer+len,mReadPtr-len);
        mReadPtr-=len;
    }else{
        memcpy(buf,m_rBuffer,mReadPtr);
        ret=mReadPtr;
        mReadPtr=0;
    }
    return ret;

}

int Connection::Write(char* buf,int len){
    int ret;
    if(len>WRITE_BUFFER_SIZE)
        return -1;
    if(len<=WRITE_BUFFER_SIZE-mWritePtr){
        memcpy(m_wBuffer+mWritePtr,buf,len);
        mWritePtr+=len;
        ret=len;
    }else{
        return -1;
    }
}


int Connection::readBuffer() {
    if(mReadPtr>=READ_BUFFER_SIZE){
        return -1;
    }
    int last_ptr=mReadPtr;
    int read_bytes;
    while(mReadPtr<READ_BUFFER_SIZE){
        //TODO len=0的时候，且缓冲区有数据recv返回什么？
        read_bytes=recv(m_sockfd,m_rBuffer+mReadPtr,READ_BUFFER_SIZE-mReadPtr,0);
        if(read_bytes<0){
            //缓存区读完了
            if(errno==EAGAIN||errno==EWOULDBLOCK){
                break;
            }else {
                return -1;
            }
        }else if(read_bytes==0){
            return -1;  //连接被远程关闭
        }
        mReadPtr+=read_bytes;
    }
    return mReadPtr-last_ptr;
}

int Connection::writeBuffer() {
    int write_bytes;
    int write_ptr=0;
    while(write_ptr<mWritePtr){
        write_bytes = send(m_sockfd, m_wBuffer + write_ptr, mWritePtr - write_ptr, 0);
        if(write_bytes<0){
            break;
        }
        write_ptr+=write_bytes;
    }

    memcpy(m_wBuffer,m_wBuffer+write_ptr,mWritePtr-write_ptr);
    mWritePtr-=write_ptr;

    if(write_bytes<0) {
        if (errno != EAGAIN) {
            return -1;
        }
    }
    return write_ptr;
}

}//namespace net