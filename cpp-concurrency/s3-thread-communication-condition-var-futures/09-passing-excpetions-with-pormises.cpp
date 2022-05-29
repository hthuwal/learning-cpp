#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <cmath>

#include "thread_guard.h"

void throw_exception() {
    throw std::runtime_error("Number cannot be negative.");
}

void calculate_square_root(std::promise<int> &prom) {
    int x = 1;
    std::cout<< "Please, enter an integer value: ";
    try {
        std::cin>>x;
        if (x < 0) {
            throw_exception();
        }
        prom.set_value(std::sqrt(x));
    } catch(std::exception&) {
        prom.set_exception(std::current_exception());
    }
}

void print_result(std::future<int> &fut) {
    try {
        int x = fut.get();
        std::cout<< "Square root is: " << x <<"\n";
    } catch (std::exception & e) {
        std::cout<< "[Exception caught: " << e.what() << "]\n";
    }
}

void run_code() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread printing_thread(print_result, std::ref(fut));
    Thread_Guard tgp(printing_thread);

    std::thread calc_thread(calculate_square_root, std::ref(prom));
    Thread_Guard tgc(calc_thread);
}

int main() {
    run_code();
}