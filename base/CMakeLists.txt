add_library(base Mutex.cpp Thread.cpp ThreadPool.cpp Timestamp.cpp Logger.cpp LogStream.cpp Buffer.cpp FileSystem.cpp)

target_link_libraries(base pthread)
#target_include_directories(base INTERFACE logger.h Mutex.h Thread.h ThreadPool.h Timestamp.h Singleton.h)
