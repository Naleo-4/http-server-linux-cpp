cmake_minimum_required(VERSION 3.13)

project(http-server-starter-cpp)
#include_directories(include)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

#add_executable(server ${SOURCE_FILES}
#        src/server.h
#        src/http_server.cpp
#        src/http_server.h
#        src/http_client.cpp
#        src/http_client.h
#        src/thread_pool.cpp
#        src/thread_pool.h
#)

add_executable(example_chat ${SOURCE_FILES}
    example_chat.cpp
)

#target_link_libraries(server PRIVATE Threads::Threads)