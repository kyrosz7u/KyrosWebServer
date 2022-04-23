//
// Created by 樱吹雪 on 2022/4/6.
//

#include "base/Logger.h"
#include "base/ThreadPool.h"
#include <unistd.h>
#include <atomic>

using namespace base;

volatile atomic<int> i(0);
//int i=0;
//Mutex locker;

void logfunc(){
    // 发现一个问题：
    // LOG_TRACE << "i= " << i; i++;
    // 按这个顺序执行代码会出现线程安全错误，似乎是与CPU的乱序执行有关系
    //
    while(1){
//        locker.Lock();
        LOG_TRACE << "i= " << ++i;
//        locker.Unlock();
        usleep(300000);
    }
}

int main(){
    ThreadPool p(4);
    p.Start();

    for(int cnt=0;cnt<10;cnt++){
        p.AddTask(logfunc);
    }


    while(1);

    return 0;
}

