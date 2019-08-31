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

Task::Task() {
    
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
                Task task;
                {
                    std::unique_lock<std::mutex> queueLock(this->mQueueMutex);
                    this->mCondVar.wait(queueLock, [this]() {
                        std::cout << "Checking\n";
                        return !this->mTasks.empty() || this->mShouldStop;
                    });
                    if (this->mShouldStop && this->mTasks.empty()) return;
                    task = this->mTasks.front();
                    this->mTasks.pop_front();
                }
                task.execute();
            }
        });
        mThreads.push_back(std::move(t));
    }
};

ThreadPool::~ThreadPool() {
    mCondVar.notify_all();
    for (auto& thread: mThreads) {
        thread.join();
    }
}

int ThreadPool::addTask(taskFn fn) {
    std::unique_lock<std::mutex> queueLock(mQueueMutex);
    if (mShouldStop) {
        std::cout << "Cannot add more tasks to queue\n";
        return -1;
    }
    std::cout << "Adding task to queue: " << taskCounter << std::endl;
    auto t = Task(fn, taskCounter++);
    mTasks.push_back(t); // does t goes out of scope?
    mCondVar.notify_one();
    return t.getId();
}

bool ThreadPool::isEmpty() {
    return mTasks.empty();
}

DeleteResponse ThreadPool::deleteTask(int taskId) {
    std::unique_lock<std::mutex> queueLock(mQueueMutex);
    for (auto t = mTasks.begin(); t != mTasks.end();) {
        if (t->getId() == taskId) {
            mTasks.erase(t);
            return DeleteResponse::DELETED;
        }
        ++t;
    }
    return DeleteResponse::NOT_FOUND;
}

void ThreadPool::stop() {
    std::unique_lock<std::mutex> queueLock(mQueueMutex);
    mShouldStop = true;
    mCondVar.notify_all();
}
