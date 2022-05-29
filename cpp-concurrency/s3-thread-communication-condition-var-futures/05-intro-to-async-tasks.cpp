#include <future>
#include <iostream>
#include <thread>

void printing() {
    std::cout << "printing runs on thread ID: " << std::this_thread::get_id() << "\n";
}

int addition(int x, int y) {
    std::cout << "adding runs on thread ID: " << std::this_thread::get_id() << "\n";
    return x + y;
}

int subtract(int x, int y) {
    std::cout << "subtraction runs on thread ID: " << std::this_thread::get_id() << "\n";
    return x - y;
}

void run_code() {
    std::cout << "Main thread ID: " << std::this_thread::get_id() << "\n";

    int x = 100;
    int y = 50;

    // This will be run as separate thread.
    std::future<void> print_task = std::async(std::launch::async, printing);

    // This will run  in main thread when add_task.get() is called.
    std::future<int> add_task = std::async(std::launch::deferred, addition, x, y);

    // Compiler decides where it will run
    std::future<int> sub_task = std::async(std::launch::async | std::launch::deferred, subtract, x, y);

    print_task.get();
    std::cout << "value received from additon task " << add_task.get() << "\n";
    std::cout << "value received from subtraction task " << sub_task.get() << "\n";
}

int main() {
    run_code();
}