//
// Created by 樱吹雪 on 2022/1/17.
//

#include <iostream>
#include <unistd.h>
#include "base/Mutex.h"
#include "base/Condition.h"
#include "base/Thread.h"
#include "base/ThreadPool.h"

using namespace std;
using namespace base;

Mutex taskLock;

int taskNum=0;

void task(){
    cout<<"TID:"<<CurrentThread::getStr()<<"    TaskNum:"<<taskNum<<endl;
    taskLock.Lock();
    taskNum++;
    taskLock.Unlock();
    sleep(5);
    cout<<"TID:"<<CurrentThread::getStr()<<"  TaskEnd!"<<endl;
}

int main(){
    ThreadPool threads(5);



//    cout<<&threads<<endl;
//    cout<<threads.isRunning<<endl;

    while(1){
        threads.AddTask(task);
        sleep(1);
    }

    threads.Start();

    return 0;
}



