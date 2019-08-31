#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

int main()
{
    auto func = [] (int taskId) {
        std::cout << "Sleeping: " << taskId << "\n";
        std::this_thread::sleep_for (std::chrono::milliseconds(2000));
        std::cout << "Task is done: " << taskId << std::endl;
    };
    
    ThreadPool pool{20};
    for (int i = 0; i < 30; i++) {
        pool.addTask(func);
    };
    
    return 0;
}
