add_library(net HttpServer.cpp Server.cpp Connection.cpp)

target_link_libraries(net base)

#target_include_directories(net INTERFACE Connection.h)

