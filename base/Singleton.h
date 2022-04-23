//
// Created by 樱吹雪 on 2022/4/2.
//

#ifndef KYROSWEBSERVER_SINGLETON_H
#define KYROSWEBSERVER_SINGLETON_H

#include <assert.h>

// 注意：这个Singleton不是线程安全的，多线程调用的时候可能会构造两个T
template<class T>
class Singleton{
public:
    GetInstance(){
        if(_instance== nullptr){
            _instance=new T;
        }
        return _instance;
    }
    Destroy(){
        assert(_instance!= nullptr);
        delete _instance;
    }
private:
    Singleton();
    static T *_instance;
};

template<class T>
Singleton<T>::_instance = nullptr;

#endif //KYROSWEBSERVER_SINGLETON_H
