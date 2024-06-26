cmake_minimum_required(VERSION 3.13)

project(http-server-starter-cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(include)
link_directories(${CMAKE_SOURCE_DIR}/include/lib)

#find_package(Threads REQUIRED)
#find_package(GameNetworkingSockets CONFIG REQUIRED)

#find_library(GameNetworkingSockets NAMES GameNetworkingSockets PATHS ${CMAKE_SOURCE_DIR}/include/lib)


add_executable(example_chat
#                ${SOURCE_FILES}
        src/example_chat.cpp
)
target_link_libraries(example_chat GameNetworkingSockets)

add_executable(server
#        ${SOURCE_FILES}
        src/server.h
        src/server.cpp
        src/http_server.cpp
        src/http_server.h
        src/http_client.cpp
        src/http_client.h
        src/thread_pool.cpp
        src/thread_pool.h
)
#target_link_libraries(server PRIVATE Threads::Threads)
