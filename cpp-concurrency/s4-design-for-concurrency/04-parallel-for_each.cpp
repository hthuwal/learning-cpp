#include <parallel/algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <execution>

#include "thread_guard.h"

const size_t testSize = 10000;

using std::milli;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

void print_results(const std::string& title, const std::vector<int>& ints, const high_resolution_clock::time_point& start_time, const high_resolution_clock::time_point& end_time) {
    printf("%s: Time: %fms\n", title.c_str(), duration_cast<duration<double, milli>>(end_time - start_time));
}

template <typename Iterator, typename Func>
void parallel_for_each_pt(Iterator first, Iterator last, Func f) {
    unsigned const long length = std::distance(first, last);
    if (length == 0) {
        return;
    }

    // Calculate optimum concurrency level
    unsigned const long min_per_thread = 25;
    unsigned const long max_threads = (length + min_per_thread - 1) / min_per_thread;

    unsigned const long hardware_threads = std::thread::hardware_concurrency();
    unsigned const long num_threads = std::min(hardware_threads, max_threads);
    unsigned const long block_size = length / num_threads;

    std::vector<std::future<void>> futures(num_threads);
    std::vector<std::thread> threads(num_threads);
    Join_Threads joiner(threads);

    Iterator block_start = first;
    for (int i = 0; i <= num_threads - 1; i++) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);

        std::packaged_task<void(void)> task([=] {
            std::for_each(block_start, block_end, f);
        });

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task));

        block_start = block_end;
    }

    for (auto& each : futures) {
        each.get();
    }
}

// In case the maximum level of concurrency is automatically controlled
template <typename Iterator, typename Func>
void parallel_for_each_async_tasks(Iterator first, Iterator last, Func f) {
    unsigned const long length = std::distance(first, last);
    if (length == 0) {
        return;
    }

    if (length * 2 < 25) {
        std::for_each(first, last, f);
    } else {
        const auto mid = first + length / 2;
        std::future<void> first_half = std::async(&parallel_for_each_async_tasks<Iterator, Func>, first, mid, f);
        parallel_for_each_async_tasks(mid, last, f);
        first_half.get();
    }
}

int main() {
    std::random_device rd;
    std::vector<int> ints(testSize);
    for (auto& num : ints) {
        num = 1 + rd() % testSize;
    }

    auto function = [](int& n) {
        int sum = 0;
        for (auto i = 0; i < 100000; i++) {
            sum += i * (i - 499);
        }
        n = sum / n;
    };

    {
        std::vector<int> curr(ints);
        auto startTime = high_resolution_clock::now();
        std::for_each(curr.begin(), curr.end(), function);
        auto endTime = high_resolution_clock::now();
        print_results("Normal For Each", curr, startTime, endTime);
    }

    {
        std::vector<int> curr(ints);
        auto startTime = high_resolution_clock::now();
        std::for_each(std::execution::seq, curr.begin(), curr.end(), function);
        auto endTime = high_resolution_clock::now();
        print_results("STL-seq", curr, startTime, endTime);
    }

    {
        std::vector<int> curr(ints);
        auto startTime = high_resolution_clock::now();
        std::for_each(std::execution::par, curr.begin(), curr.end(), function);
        auto endTime = high_resolution_clock::now();
        print_results("STL-par", curr, startTime, endTime);
    }

    {
        std::vector<int> curr(ints);
        auto startTime = high_resolution_clock::now();
        parallel_for_each_pt(curr.begin(), curr.end(), function);
        auto endTime = high_resolution_clock::now();
        print_results("Parallel Package Tasks", curr, startTime, endTime);
    }

    {
        std::vector<int> curr(ints);
        auto startTime = high_resolution_clock::now();
        parallel_for_each_async_tasks(curr.begin(), curr.end(), function);
        auto endTime = high_resolution_clock::now();
        print_results("Parallel Async Tasks", curr, startTime, endTime);
    }
}