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
#include <mutex>
#include <deque>
#include <condition_variable>

using taskFn = std::function<void(int taskId)>;

class Task {
    taskFn mFn;
    int mId;
public:
    Task(taskFn fn, int taskId);
    Task();
    void execute();
    int getId();
};

enum class DeleteResponse {
    DELETED,
    NOT_FOUND
};

class ThreadPool {
    std::deque<Task> mTasks;
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
    DeleteResponse deleteTask(int taskId);
};

#endif /* ThreadPool_h */
