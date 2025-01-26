#pragma once
#ifndef TASK_H
#define TASK_H

#include <fmt/format.h>
#include <iostream>
#include <vector>
#include <future>
#include <queue>
#include <thread>
#include <atomic>
#include <functional>
#include <unordered_map>
#include <utility> // for std::forward
#include <chrono>

class TaskManager
{
public:
    using Callback = std::function<void(std::unordered_map<std::string, std::string>)>; // 回调函数类型

    TaskManager(int max_concurrent_tasks, int sleep_time)
        : max_concurrent_tasks_(max_concurrent_tasks), running_tasks_(0)
    {
        this->sleep_time = sleep_time;
    }

    // 添加任务，支持自定义函数、参数和回调函数
    template <typename Func, typename... Args>
    void add_task(Func &&func, Callback callback, Args &&...args)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]()
                 { return running_tasks_ < max_concurrent_tasks_; });

        auto task = [this, func = std::forward<Func>(func), callback, args...]() mutable
        {
            try
            {
                auto result = func(args...); // 调用函数并获取返回值
                if (callback)
                {
                    if (result.empty())
                    {
                        return;
                    }
                    callback(result); // 将返回值传递给回调函数
                }
            }
            catch (const std::exception &e)
            {
                // 处理异常，例如，通过回调函数传递异常信息
                fmt::print("Exception caught in task: {}\n", e.what());
            }
            catch (...)
            {
                // 处理未知异常，例如，通过回调函数传递异常信息
                fmt::print("Unknown exception caught in task\n");
            }

            std::this_thread::sleep_for(std::chrono::seconds(this->sleep_time)); // 休眠
            std::unique_lock<std::mutex> lock(mutex_);
            --running_tasks_;
            cv_.notify_one(); // 通知可以添加新任务
        };

        tasks_.push(std::async(std::launch::async, std::move(task)));
        ++running_tasks_;
    }

    void wait_all()
    {
        while (!tasks_.empty())
        {
            tasks_.front().wait();
            tasks_.pop();
        }
    }

private:
    int sleep_time;

    int max_concurrent_tasks_;
    std::atomic<int> running_tasks_;
    std::queue<std::future<void>> tasks_;
    std::mutex mutex_;
    std::mutex callback_mutex_; // 保护回调函数调用
    std::condition_variable cv_;
};

#endif // TASK_H