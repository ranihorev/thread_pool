//
//  ThreadPool.h
//  learncpp
//
//  Created by Rani Horev on 8/30/19.
//  Copyright © 2019 Rani Horev. All rights reserved.
//

#ifndef ThreadPool_h
#define ThreadPool_h

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using taskFn = std::function<void(int taskId)>;

class Task {
    taskFn mFn;
    int mId;
public:
    Task(taskFn fn, int taskId);
    void execute();
    int getId();
};

class ThreadPool {
    std::queue<Task> mTasks;
    std::vector<std::thread> mThreads;
    const int mSize;
    int taskCounter = 0;
    std::mutex mQueueMutex;
    std::condition_variable mCondVar;
    
public:
    ThreadPool(int size);
    ~ThreadPool();
    int addTask(taskFn fn);
    bool isEmpty();
};

#endif /* ThreadPool_h */
