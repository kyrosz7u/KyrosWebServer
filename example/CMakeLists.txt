#SET(EXECUTABLE_OUTPUT_PATH ./bin)

add_executable(thread_test thread_test.cpp)
target_link_libraries(thread_test base)

add_executable(timestamp_test timestamp_test.cpp)
target_link_libraries(timestamp_test base)

add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test base)

add_executable(threadpool_test threadpool_test.cpp)
target_link_libraries(threadpool_test base)

add_executable(echoserver_test echoserver_test.cpp)
target_link_libraries(echoserver_test base net)
