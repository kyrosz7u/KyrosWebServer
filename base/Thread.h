//
// Created by 樱吹雪 on 2022/1/11.
//

#ifndef KYROSWEBSERVER_TREAD_H
#define KYROSWEBSERVER_TREAD_H

#include "utils.h"
#include <string>
#include <functional>

using namespace std;

namespace base{

//typedef void*(*ThreadTask)(void*);

typedef std::function<void()> ThreadTask;

namespace CurrentThread{
    extern __thread pid_t Tid;
    extern __thread char TidStr[16];

    pid_t getTid();
    char const* getStr();
};

struct ThreadData{
    string mTidStr="";
    string mName="";
    ThreadTask mTask;
};

class Thread:noncopyable{
public:
    Thread(ThreadTask t);
    Thread(ThreadTask t, const string &name);
    ~Thread();
    void start();
    void join();
    pid_t tid();
private:
    ThreadData runningData;
    pthread_t mPthreadId=0;
    bool isRunning=false;
    bool isJoin=false;
};

};

#endif
