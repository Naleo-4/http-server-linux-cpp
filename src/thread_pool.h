//
// Created by kali on 6/25/24.
//

#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class Thread_pool {
public:
    Thread_pool(size_t numth);
    ~Thread_pool();
    void enqueue(std::function<void()> task);
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

    void worker_thread();
};



#endif //THREAD_POOL_H
