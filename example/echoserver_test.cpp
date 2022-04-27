//
// Created by 樱吹雪 on 2022/4/5.
//

#include "net/Server.h"
#include "base/LogStream.h"

using namespace std;
using namespace net;

void cb(ConnPtr & c);

int main()
{
    Server s(23333,4);

    s.setReadCallBack(cb);
    s.EventLoop();
    return 0;
}

void cb(ConnPtr& c){
    char buf[128];
    int n=c->Read(buf,128);
    c->Write(buf, n);
    c->enableConnWrite();
}
