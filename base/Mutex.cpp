//
// Created by 樱吹雪 on 2022/1/13.
//

#include "Mutex.h"
#include <assert.h>


namespace base {
Mutex::Mutex() {
    int ret = pthread_mutex_init(&mMutex, NULL);
    assert(ret == 0);
}

Mutex::~Mutex() {
    int ret = pthread_mutex_destroy(&mMutex);
    assert(ret == 0);
}

void Mutex::Lock() {
    int ret = pthread_mutex_lock(&mMutex);
    assert(ret == 0);
    isLocked = true;
}

void Mutex::Unlock() {
    int ret = pthread_mutex_unlock(&mMutex);
    assert(ret == 0);
    isLocked = false;
}
}//namespace base






