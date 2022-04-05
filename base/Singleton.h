//
// Created by 樱吹雪 on 2022/4/2.
//

#ifndef KYROSWEBSERVER_SINGLETON_H
#define KYROSWEBSERVER_SINGLETON_H

template<class T>
class Singleton{
public:
    GetInstance(){
        if(_instance== nullptr){
            _instance=new T;
        }
        return _instance;
    }
private:
    Singleton();
    static T *_instance;
};

template<class T>
Singleton<T>::_instance = nullptr;

#endif //KYROSWEBSERVER_SINGLETON_H
