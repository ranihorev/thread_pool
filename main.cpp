#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

int main()
{
    auto func = [] (int taskId) {
        std::cout << "Sleeping: " << taskId << "\n";
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
        std::cout << "Task is done: " << taskId << std::endl;
    };
    
    ThreadPool pool{4};
    for (int i = 0; i < 8; i++) {
        pool.addTask(func);
    };
    int task_to_delete = pool.addTask(func);
    auto success = pool.deleteTask(task_to_delete);
    std::cout << "Task was delete: " << (success == DeleteResponse::DELETED) << std::endl;
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    pool.addTask(func);
    pool.addTask(func);
    std::this_thread::sleep_for (std::chrono::milliseconds(2500));
    pool.stop();
    
    return 0;
}
