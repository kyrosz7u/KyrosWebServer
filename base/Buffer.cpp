//
// Created by 樱吹雪 on 2022/4/15.
//

#include "Buffer.h"
#include <sys/socket.h>

namespace base {

// 写入字符串
int FixedBuffer::append(const char *str, size_t len) {
    if (static_cast<size_t>(_end - mWritePtr) > len) {
        memcpy(mWritePtr, str, len);
    }
        // 腾挪空间
    else if (avail() >= len) {
        size_t used_len = used();
        memcpy(_buf, mReadPtr, used_len);
        mReadPtr = _buf;
        mWritePtr = _buf + used_len;
        memcpy(mWritePtr, str, len);
    } else {
        return 0;
    }
    mWritePtr += len;
    return len;
}


/* StringPiece中ptr的类型是const，所以使用这个类也要加const */
int FixedBuffer::append(const StringPiece &str)
{
    return append(str.data(), str.size());
}

//// 接收字符串
//int FixedBuffer::append(std::string &str)
//{
//    return append(str.c_str(), str.length());
//}

/*写入字符*/
int FixedBuffer::append(const char &ch)
{
    return append(&ch, 1);
}

/*读取socketfd到buffer*/
int FixedBuffer::readFd(int socketfd)
{
    if(avail()==0){
        return 0;
    }

    int read_bytes;
    size_t last_used=used();
    while(avail()>0){
        size_t len=static_cast<size_t>(_end - mWritePtr);
        /*腾挪空间*/
        if(len==0){
            size_t used_len = used();
            memcpy(_buf, mReadPtr, used_len);
            mReadPtr = _buf;
            mWritePtr = _buf + used_len;
            continue;
        }
        //TODO len=0的时候，且缓冲区有数据recv返回什么？
        read_bytes=::recv(socketfd,mWritePtr,len,0);
        if(read_bytes<0){
            //缓存区读完了
            if(errno==EAGAIN||errno==EWOULDBLOCK){
                break;
            }else {
                return -1;
            }
            //连接被远程关闭
        }else if(read_bytes==0){
            return -1;
        }
        mWritePtr+=read_bytes;
    }
    return used()-last_used;
}


}//namespace base