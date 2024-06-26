//
// Created by kali on 6/25/24.
//

#include "thread_pool.h"
Thread_pool::Thread_pool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&Thread_pool::worker_thread, this);
    }
}

Thread_pool::~Thread_pool() {
    {
        std::unique_lock<std::mutex> lock(Thread_pool);
        stop = true;
    }
    condition.notify_all();
    for (std::jthread &worker : workers) {
        worker.join();
    }
}

void Thread_pool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(std::move(task));
    }
    condition.notify_one();
}

void Thread_pool::worker_thread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}