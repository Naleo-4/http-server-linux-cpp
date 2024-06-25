//
// Created by kali on 6/25/24.
//

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

class ThreadPool
{
public:
    // // Constructor to creates a thread pool with given
    // number of threads
    ThreadPool(size_t num_threads
        = std::thread::hardware_concurrency())
    {
        // Creating worker threads
        for (size_t i = 0; i < num_threads; ++i)
        {
            threads_.emplace_back([this]
            {
                while (true)
                {
                    std::function<void()> task;
                    // The reason for putting the below code
                    // here is to unlock the queue before
                    // executing the task so that other
                    // threads can perform enqueue tasks
                    {
                        // Locking the queue so that data
                        // can be shared safely
                        std::unique_lock<std::mutex> lock(
                            queue_mutex_);

                        // Waiting until there is a task to
                        // execute or the pool is stopped
                        cv_.wait(lock, [this]
                        {
                            return !tasks_.empty() || stop_;
                        });

                        // exit the thread in case the pool
                        // is stopped and there are no tasks
                        if (stop_ && tasks_.empty())
                        {
                            return;
                        }

                        // Get the next task from the queue
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }

                    task();
                }
            });
        }
    }

    // Destructor to stop the thread pool
    ~ThreadPool()
    {
        {
            // Lock the queue to update the stop flag safely
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }

        // Notify all threads
        cv_.notify_all();

        // Joining all worker threads to ensure they have
        // completed their tasks
        for (auto& thread : threads_)
        {
            thread.join();
        }
    }

    // Enqueue task for execution by the thread pool
    void enqueue(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.emplace(move(task));
        }
        cv_.notify_one();
    }

private:
    // Vector to store worker threads
    std::vector<std::thread> threads_;

    // Queue of tasks
    std::queue<std::function<void()>> tasks_;

    // Mutex to synchronize access to shared data
    std::mutex queue_mutex_;

    // Condition variable to signal changes in the state of
    // the tasks queue
    std::condition_variable cv_;

    // Flag to indicate whether the thread pool should stop
    // or not
    bool stop_ = false;
};

// template <typename T>
class Http_server
{
public:
    const static Http_server* instance;
    const int server_fd;
    const int reuse = 1;
    const int connection_backlog = 5;
    const sockaddr_in server_addr
    {
        .sin_family = AF_INET,
        .sin_port = htons(4221),
        .sin_addr = {.s_addr = INADDR_ANY},
        .sin_zero{0}
    };

private:
    Http_server() : server_fd(socket(AF_INET, SOCK_STREAM, 0))
    {
        std::cout << std::unitbuf;
        std::cerr << std::unitbuf;

        if (server_fd < 0)
        {
            std::cerr << "Failed to create server socket\n";
            // return 1;
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
        {
            std::cerr << "setsockopt failed\n";
            // return 1;
        }

        if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) != 0)
        {
            std::cerr << "Failed to bind to port 4221\n";
            // return 1;
        }
        if (listen(server_fd, connection_backlog) != 0)
        {
            std::cerr << "listen failed\n";
            // return 1;
        }
    }

public:
    static const Http_server* get_instance()
    {
        if (!instance)
        {
            instance = new Http_server();
        }
        return instance;
    }
};
#endif //HTTP_SERVER_H
