//
//  ThreadPool.hpp
//  ThreadPool
//
//  Created by AlexiChen on 2017/9/15.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <stdio.h>

#include <iostream>
#include <thread>
#include <functional>
#include <future>
#include <atomic>
#include <queue>
#include <vector>

class ThreadPool
{
    
    using Task = std::function<void()>;
private:
    std::vector<std::thread> mPool;
    std::queue<Task> mTaskQueue;
    std::mutex mTaskMutex;
    std::condition_variable mTaskCV;
    
    std::atomic<bool> mStop;
    
public:
    ThreadPool(size_t size = 10) : mStop(false)
    {
        size = size < 1 ? 1 : size;
        for (size_t i = 0; i < size; i++)
        {
            mPool.emplace_back(&ThreadPool::schedual, this);
        }
    }
    
    
    ~ThreadPool()
    {
        for (std::thread &thread : mPool)
        {
            thread.detach();
        }
    }

    
    void shutdown()
    {
        mStop.store(true);
    }
    
    void restart()
    {
        mStop.store(false);
    }
    
public:
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        if(mStop.load())
        {
            throw std::runtime_error("task executor have closed commit.");
        }
        
        using TaskReturnType =  decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<TaskReturnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        
        // 添加任务到队列
        std::lock_guard<std::mutex> lock(mTaskMutex);
        mTaskQueue.emplace([task]() {
            (*task)();
        });
        
        mTaskCV.notify_all();    // 唤醒线程执行
        
        std::future<TaskReturnType> future = task->get_future();
        return future;
    }
    
    
public:
    void schedual()
    {
        while (true)
        {
            if (Task task = getTask())
            {
                task();
            }
            else
            {
                // do nothing;
            }
        }
    }
    
public:
    Task getTask()
    {
        std::unique_lock<std::mutex> lock(mTaskMutex);
        mTaskCV.wait(lock, [this]{
            return !mTaskQueue.empty();
        });
        
        Task task(std::move(mTaskQueue.front()));
        
        mTaskQueue.pop();
        
        return task;
    }
    
};


#endif /* ThreadPool_hpp */
