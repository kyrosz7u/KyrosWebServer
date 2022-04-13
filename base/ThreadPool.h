//
// Created by 樱吹雪 on 2022/1/16.
//

#ifndef KYROSWEBSERVER_THREADPOOL_H
#define KYROSWEBSERVER_THREADPOOL_H

#include "Thread.h"
#include "Condition.h"
#include "Mutex.h"

#include <vector>
#include <queue>
#include <functional>

using namespace std;

namespace base {

class ThreadPool {
public:
    ThreadPool(int _workers);

    ThreadPool(int _workers, int _qSize);

    ~ThreadPool();

    void Start();

    void Stop();

    void AddTask(ThreadTask &&t);

    bool isRunning = false;
private:
    ThreadTask take();
    void runInThread();

    int workers;
    int qSize;
    Mutex mMutex;
    Condition isFull;
    Condition isEmpty;
    queue<ThreadTask> mTasks;
    vector<Thread *> mThreads;
};

}

#endif
