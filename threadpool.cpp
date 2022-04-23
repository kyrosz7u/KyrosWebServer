#include <iostream>
#include <unistd.h>
#include "base/Thread.h"
#include "base/Mutex.h"
#include "base/LogStream.h"

using namespace std;
using namespace base;

Mutex mutex;

#define NLOOP 100000
int a=0;

void thr_fn1(){
    int i=0;
    while(i<NLOOP){
        mutex.Lock();
        cout<<"thread t1:a="<<a<<endl;
        a++;
        i++;
        usleep(5);
        mutex.Unlock();
        LOG_DEBUG("123");
    }
}


int main() {
    pthread_t tid;
    Thread t1(thr_fn1);
//    Thread t2(thr_fn1,"t2");

    t1.start();
//    t2.start();

    t1.join();
//    t2.join();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

