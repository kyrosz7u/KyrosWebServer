//
// Created by 樱吹雪 on 2022/1/13.
//

#ifndef KYROSWEBSERVER_MUTEX_H
#define KYROSWEBSERVER_MUTEX_H


#include <pthread.h>

namespace base {

class Mutex {
public:
    Mutex();

    ~Mutex();

    void Lock();

    void Unlock();

private:
    friend class Condition;

    Mutex(const Mutex &mutex);

    pthread_mutex_t mMutex;
    bool isLocked = false;
};

class MutexLock {
public:
    MutexLock(Mutex &mutex)
            : _Mutex(mutex) {
        _Mutex.Lock();
    }

    ~MutexLock() {
        _Mutex.Unlock();
    }

private:
    Mutex &_Mutex;
};
}//namespace base {

#endif //LINUX_SYS_MUTEX_H


