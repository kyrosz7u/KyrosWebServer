//
// Created by 樱吹雪 on 2022/4/4.
//

#ifndef KYROSWEBSERVER_CONNECTION_H
#define KYROSWEBSERVER_CONNECTION_H

#include <unistd.h>
#include <functional>
#include <memory>

namespace net {
//系统为每个连接分配固定大小的读写缓冲区
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
class Connection {
public:
    Connection(int socketfd)
    :m_sockfd(socketfd),
    mReadPtr(0),
    mWritePtr(0){}
    ~Connection(){close(m_sockfd);}
    int Read(char* buf,int len);
    int Write(char* buf,int len);
public:
//    friend class Server;
    int readBuffer();
    int writeBuffer();
    char m_rBuffer[READ_BUFFER_SIZE];
    int mReadPtr;
    char m_wBuffer[WRITE_BUFFER_SIZE];
    int mWritePtr;
    int m_sockfd;
};
}//namespace net

#endif //KYROSWEBSERVER_CONNECTION_H
