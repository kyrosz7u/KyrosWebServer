//
// Created by 樱吹雪 on 2022/1/16.
//

#include "ThreadPool.h"
#include "Logger.h"
#include <assert.h>

using namespace std;
using namespace base;

namespace base{
ThreadPool::ThreadPool(int _workers)
        : workers(_workers),
          qSize(2 * _workers),
          isFull(std::move(Condition(mMutex))),
          isEmpty(std::move(Condition(mMutex))) {};

ThreadPool::ThreadPool(int _workers, int _qSize)
        : workers(_workers),
          qSize(_qSize),
          isFull(std::move(Condition(mMutex))),
          isEmpty(std::move(Condition(mMutex))) {};

ThreadPool::~ThreadPool(){
    if(isRunning){
        Stop();
    }

}

void ThreadPool::Start(){
    isRunning= true;
    for(int i=0;i<workers;i++){
        //这样会不会内存泄露？mThreads析构的时候好像不会执行Thread的析构函数
        Thread *t = new Thread(std::bind(&ThreadPool::runInThread,this),"null");
        t->start();
        mThreads.push_back(t);
    }
}

void ThreadPool::Stop(){
    MutexLock _ML(mMutex);
    isRunning= false;
    //唤醒被阻塞的线程
    isFull.postAll();
    isEmpty.postAll();
    for(auto t:mThreads){
        t->join();
    }
}

void ThreadPool::AddTask(ThreadTask &&t){
//    assert(isRunning == 1);     //不然会无法添加任务
    MutexLock _ML(mMutex);
//    if(isRunning){
    while(mTasks.size()>=qSize){
        isFull.wait();
    }
    mTasks.push(std::move(t));
    if(mTasks.size()==1){
        isEmpty.post();
    }
//    }
}

ThreadTask ThreadPool::take(){
    MutexLock _ML(mMutex) ;
    ThreadTask t;

    while(mTasks.size()<=0){
        isEmpty.wait();
    }
    //线程池析构的时候会isRunning=false
    if(isRunning){
        t = mTasks.front();
        mTasks.pop();
    }else{
        t= nullptr;
    }
    if(mTasks.size()==qSize-1){
        isFull.post();
    }
    return t;
}

void ThreadPool::runInThread(){
//    cout<<"thread start:"<<CurrentThread::getStr()<<"this:"<<this<<endl;
//    cout<<this->isRunning<<endl;
//    cout<<"thread start:"<<CurrentThread::getStr()<<endl;
    while(isRunning){
        ThreadTask t=take();
        if(t){
            t();
        }

    }
}
}//namespace base;



