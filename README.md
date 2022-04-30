# KyrosWebServer

## 概要

1、网络库基于Reactor模式，IO多路复用采用epoll的ET模式；

2、使用基于阻塞队列的线程池并发处理请求；

3、网络连接采用非阻塞IO，通过用户态缓冲来提高上层的代码的响应速度；

## 依赖

boost==1.75.0

gtest==1.10

## 参考

Linux高性能服务器编程，游双.

Linux多线程服务端编程：使用muduo C++网络库，陈硕.

[muduo](https://github.com/chenshuo/muduo)