#include <future>
#include <iostream>
#include <thread>

void print_result(std::future<int>& fut) {
    // std::cout << fut.get() << "\n";  This will cause problem only one thread will able to get, rest will throw exception

    // This can lead to race condition.
    if (fut.valid()) {
        std::cout <<"This is valid future\n" << fut.get() << "\n";
    }
    else {
        std::cout<< "This is invalid future\n";
    }
}

void run_code() {
    std::promise<int> prom;
    std::future<int> fut(prom.get_future());

    std::thread th1(print_result, std::ref(fut));
    std::thread th2(print_result, std::ref(fut));

    prom.set_value(5);

    th1.join();
    th2.join();
}

int main() {
    run_code();
}