#include <future>
#include <iostream>
#include <thread>

#include "thread_guard.h"

void print_int(std::future<int>& fut) {
    std::cout << "Waiting for value to be set \n";
    std::cout << "Got Value: " << fut.get() << "\n";
}

void run_code() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::thread print_thread(print_int, std::ref(fut));
    Thread_Guard tg(print_thread);

    std::cout << "Sleeping for 5 seconds. Will set value afterwards.\n"; 
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "Setting the value to be printed\n";
    prom.set_value(42);
}

int main() {
    run_code();
}