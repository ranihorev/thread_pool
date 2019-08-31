//
//  ThreadPool.cpp
//  learncpp
//
//  Created by Rani Horev on 8/30/19.
//  Copyright © 2019 Rani Horev. All rights reserved.
//
#include <thread>
#include <chrono>
#include <iostream>
#include "ThreadPool.h"

Task::Task(taskFn fn, int taskId): mFn{fn}, mId{taskId} {
    
}

void Task::execute() {
    mFn(mId);
}

int Task::getId() {
    return mId;
}

ThreadPool::ThreadPool(int size): mSize{size} {
    for(int i = 0; i < size; i++) {
        auto t = std::thread( [this, i](){
            while (true) {
                Task* task;
                {
                    std::unique_lock<std::mutex> queueLock(this->mQueueMutex);
                    this->mCondVar.wait(queueLock, [this]() { return !this->mTasks.empty(); });
                    task = &this->mTasks.front();
                    this->mTasks.pop();
                    std::cout << "Thread " << i << " Executing task " << task->getId() << std::endl;
                }
                task->execute();
            }
        });
        mThreads.push_back(std::move(t));
    }
};

ThreadPool::~ThreadPool() {
//    mCondVar.notify_all();
    for (auto& thread: mThreads) {
        thread.join();
    }
}

int ThreadPool::addTask(taskFn fn) {
    std::cout << "Adding task to queue: " << taskCounter << std::endl;
    {
        std::unique_lock<std::mutex> queueLock(mQueueMutex);
        mTasks.push(Task(fn, taskCounter++));
        mCondVar.notify_one();
    }
    return taskCounter;
}

bool ThreadPool::isEmpty() {
    return mTasks.empty();
}

