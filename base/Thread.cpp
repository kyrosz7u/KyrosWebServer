//
// Created by 樱吹雪 on 2022/1/11.
//
#include "Thread.h"
#include <pthread.h>
#include <assert.h>
#include <functional>

namespace base {

namespace CurrentThread {
__thread pid_t Tid = 0;
__thread char TidStr[16] = "";
}

Thread::Thread(ThreadTask t, const string &name) {
    runningData.mTask = std::move(t);
    runningData.mName = name;
}

Thread::Thread(ThreadTask t) {
    runningData.mTask = std::move(t);
    runningData.mName = "null";
}

Thread::~Thread() {
    if (!isJoin && isRunning) {
        int ret = pthread_detach(mPthreadId);
        assert(ret == 0);
    }
}

void *runInThread(void *args) {
    ThreadData *runningData = (ThreadData *) args;
    runningData->mTask();
    return (void *) 0;
}

void Thread::start() {
    int ret = pthread_create(&mPthreadId, NULL, runInThread, (void *) &runningData);
    assert(ret == 0);
    isRunning = true;
}

void Thread::join() {
    int ret = pthread_join(mPthreadId, NULL);
    assert(ret == 0);
    isJoin = true;
}


}//namespace base;






