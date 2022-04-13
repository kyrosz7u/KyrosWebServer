//
// Created by 樱吹雪 on 2022/1/15.
//
#include <iostream>
#include <unistd.h>
#include "base/Mutex.h"
#include "base/Condition.h"
#include "base/Thread.h"

#define BUFFER_SIZE 100

using namespace std;
using namespace base;

Mutex mutex;
Condition isFull(mutex);
Condition isEmpty(mutex);

int buffer=0;
void ProducerThread(){
    while(1){
        mutex.Lock();
        usleep(150000);
        while(buffer>=BUFFER_SIZE){
            isFull.wait();
        }

        buffer++;
        if(buffer==1){
            isEmpty.post();
        }
        if(buffer>BUFFER_SIZE){
            cout<<"error:buffer="<<buffer<<endl;
            throw "error:buffer>BUFFER_SIZE";
        }
        cout<<"Producer:"<<CurrentThread::getStr()<<" "<<buffer<<endl;

        mutex.Unlock();
    }
    return ;
}

void ConsumerThread(){
    while (1){
        mutex.Lock();
        usleep(700000);
        while(buffer<=0){
            isEmpty.wait();
        }

        buffer--;
        if(buffer==BUFFER_SIZE-1){
            isFull.post();
        }
        if(buffer<0){
            cout<<"error:buffer="<<buffer<<endl;
            throw "error:buffer<0";
        }
        cout<<"Consumer:"<<CurrentThread::getStr()<<" "<<buffer<<endl;
        mutex.Unlock();
    }
    return ;
}



int main(){
    Thread Producer(ProducerThread,"Producer");
    Thread Consumer1(ConsumerThread,"Consumer1");
    Thread Consumer2(ConsumerThread,"Consumer2");
    Thread Consumer3(ConsumerThread,"Consumer3");
    Thread Consumer4(ConsumerThread,"Consumer4");
    Thread Consumer5(ConsumerThread,"Consumer5");

    Producer.start();
    Consumer1.start();
    Consumer2.start();
    Consumer3.start();
    Consumer4.start();
    Consumer5.start();

    Producer.join();
    Consumer1.join();
    Consumer2.join();
    Consumer3.join();
    Consumer4.join();
    Consumer5.join();
    return 0;
}


