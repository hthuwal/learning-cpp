#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <execution>

#include "thread_guard.h"
using std::milli;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

void printVector(const std::vector<int> &v) {
    for (auto each: v) {
        std::cout<<each<<" ";
    }
    std::cout<<"\n";
}

void print_time_taken(const std::string& title, const high_resolution_clock::time_point& start_time, const high_resolution_clock::time_point& end_time) {
    printf("%s: Time: %fms\n", title.c_str(), duration_cast<duration<double, milli>>(end_time - start_time));
}