#include <functional>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>

int add(int x, int y) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Add function thread Id: " << std::this_thread::get_id() << "\n";
    return x + y;
}

/**
 * Running task in detached thread.
 **/
void task_thread() {
    std::packaged_task<int(int, int)> task_1(add);
    std::future<int> sum = task_1.get_future();

    // Constructed a detached thread, have to use explicit move because packaged_task is not copyable.
    std::thread thread_1(std::move(task_1), 7, 9);
    thread_1.detach();

    std::cout << "Task Thread Returned: " << sum.get() << "\n";
}

/**
 * This created task will run sequentially in current thread
 **/
void task_normal() {
    std::packaged_task<int(int, int)> task_1(add);
    std::future<int> sum = task_1.get_future();
    task_1(7, 8);
    std::cout << "Task Normal Returned: " << sum.get() << "\n";
}

void run_code() {
    std::cout << "Main Thread Id: " << std::this_thread::get_id() << "\n";
    task_thread();
    task_normal();
}

int main() {
    run_code();
}