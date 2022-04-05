//
// Created by 樱吹雪 on 2022/1/15.
//

#ifndef LINUX_SYS_CONDITION_H
#define LINUX_SYS_CONDITION_H

#include <semaphore.h>
#include "Mutex.h"

namespace base {
class Condition {
public:
    Condition(Mutex &_mutex)
            : mutex(_mutex) {
        pthread_cond_init(&cond, NULL);
    }

    int post() {
        pthread_cond_signal(&cond);
    }

    int postAll() {
        pthread_cond_broadcast(&cond);
    }

    int wait() {
        pthread_cond_wait(&cond, &mutex.mMutex);
    }

    ~Condition() { pthread_cond_destroy(&cond); }

private:
    Condition();

    pthread_cond_t cond;
    Mutex &mutex;
};
}//namespace base {
#endif //LINUX_SYS_CONDITION_H
