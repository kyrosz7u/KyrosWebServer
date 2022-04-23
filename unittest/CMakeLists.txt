add_executable(buffer_unittest buffer_unittest.cpp)
target_link_libraries(buffer_unittest base gtest)

add_executable(HttpRequest_unittest HttpRequest_unittest.cpp)
target_link_libraries(HttpRequest_unittest base net gtest)



