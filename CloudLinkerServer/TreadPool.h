#ifndef TREADPOOL_H
#define TREADPOOL_H
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<vector>
#include<future>
#include<functional>
#include<memory>
#include<iostream>
#include<atomic>
//使用future的版本

class ThreadPool
{
private:
    std::vector<std::thread>threads;
    std::queue<std::function<void()>>tasks;
    std::mutex mtx;

    std::condition_variable cv;
    std::atomic<bool> stop;

    void work()
    {
        while (true)
        {
            std::function<void()>task;

            {
                std::unique_lock<std::mutex>lock(mtx);

                cv.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop && tasks.empty())return;
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }
public:
    ThreadPool(size_t nums):stop(false)
    {
        for (size_t i = 0; i < nums; ++i)
        {

            threads.emplace_back([this]() {
                return this->work();
            });

        }

    }

    ~ThreadPool()
    {
        //{
        //	std::unique_lock<std::mutex>lock(mtx);
        stop = true;
        //}
        cv.notify_all();

        for (std::thread& oneread : threads)
        {
            oneread.join();
        }

    }

    template<typename F,typename... Args>
    auto enques(F&& f, Args...args) -> std::future<decltype(f(args...))>
    {
        using return_type = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

        std::future<return_type>res = task->get_future();
        {
            std::lock_guard<std::mutex>lock(mtx);
            if (stop)
                throw std::runtime_error("error timeout!!!");

            tasks.emplace([task]() {
                (*task)();
            });
        }
        cv.notify_one();
        return res;
    }

};

#endif // TREADPOOL_H
