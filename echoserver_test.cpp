//
// Created by 樱吹雪 on 2022/4/5.
//
#include <iostream>
#include "net/Server.h"

using namespace std;
using namespace net;

void cb(shared_ptr<Connection>& c);

int main()
{
    cout<<111<<endl;
    Server s(23333,4);

    s.setReadCallBack(cb);
    s.EventLoop();
    return 0;
}

void cb(shared_ptr<Connection>& c){
    char buf[128];
    int n=c->Read(buf,128);
    cout<<buf<<endl;
    c->Write(buf, n);
}
