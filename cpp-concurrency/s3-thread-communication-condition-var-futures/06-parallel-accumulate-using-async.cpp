#include <future>
#include <numeric>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

int MIN_ELEMENT_COUNT = 1000;

template <typename iterator>
int parallel_accumulate(iterator begin, iterator end) {
    // std::cout << "thread ID: " << std::this_thread::get_id() << "\n";
    int length = std::distance(begin, end);

    // atleast 1000 elements 
    if (length < MIN_ELEMENT_COUNT) {
        return std::accumulate(begin, end, 0);
    }

    iterator mid = begin;
    std::advance(mid, (length + 1) / 2);

    // Spawn an async task for left half
    // Launch policy -> Compiler decides run it in the same thread or spawn a new one. This is how we avoid over subscription, compiler decides this based on
    // cpu resources.
    std::future<int> left_half = std::async(std::launch::deferred | std::launch::async, parallel_accumulate<iterator>, begin, mid);

    // Call parallel accumulate for right half
    auto right_half = parallel_accumulate<iterator>(mid, end);
    return left_half.get() + right_half;
}

void run_code() {
    long n = 10000;
    std::vector<long> nums(n, 0);
    for (int i = 0; i < n; i++) {
        nums[i] = rand() % 100 + 1;
    }

    auto st = std::chrono::system_clock::now();
    std::cout << "Sequential Sum: " << std::accumulate(nums.begin(), nums.end(), 0) << "\n";
    auto et = std::chrono::system_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(et - st);
    std::cout << "Took " << dur.count() << " micro seconds\n";

    st = std::chrono::system_clock::now();
    long psum = parallel_accumulate<std::vector<long>::iterator>(nums.begin(), nums.end());
    std::cout << "Parallel Sum: " << psum << "\n";
    et = std::chrono::system_clock::now();
    dur = std::chrono::duration_cast<std::chrono::microseconds>(et - st);
    std::cout << "Took " << dur.count() << " micro seconds\n";
}

int main() {
    run_code();
}