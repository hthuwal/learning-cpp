#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "thread_guard.h"

using std::milli;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

const size_t testSize = 1000000;

template <typename Iterator, typename MatchType>
Iterator parallel_find_pt(Iterator first, Iterator last, MatchType match) {
    struct find_element {
        void operator()(Iterator begin, Iterator end, MatchType match, std::promise<Iterator> *result, std::atomic<bool> *done_flag) {
            try {
                while (true) {
                    if (std::atomic_load(done_flag)) {
                        break;
                    }

                    if (begin == end) {
                        break;
                    }

                    if (*begin == match) {
                        result->set_value(begin);
                        std::atomic_store(done_flag, true);
                        return;
                    }

                    begin++;
                }
            } catch (...) {
                result->set_exception(std::current_exception());
                done_flag->store(true);
            }
        }
    };

    unsigned long const length = std::distance(first, last);

    if (length == 0) {
        return last;
    }

    unsigned long const min_per_thread = 25;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const max_threads = length / min_per_thread + (length % min_per_thread == 0 ? 0 : 1);

    unsigned long const num_threads = std::min(hardware_threads, max_threads);
    unsigned long const block_size = length / num_threads;

    std::promise<Iterator> result;
    std::atomic<bool> done_flag(false);

    std::vector<std::thread> threads(num_threads - 1);
    {
        Join_Threads joiner(threads);

        Iterator block_start = first;
        for (int i = 0; i < num_threads - 1; i++) {
            Iterator block_end = block_start;
            std::advance(block_end, block_size);

            threads[i] = std::thread(find_element(), block_start, block_end, match, &result, &done_flag);

            block_start = block_end;
        }

        find_element()(block_start, last, match, &result, &done_flag);
    }  // When the scope ends, joiner will be destroyed. Which implies waiting on all the threads

    if (!done_flag.load()) {
        return last;
    }

    return result.get_future().get();
}

// Recursive
template <typename Iterator, typename MatchType>
Iterator parallel_find_async(Iterator first, Iterator last, MatchType match, std::atomic<bool>* done_flag) {
   try {
        unsigned long const length = std::distance(first, last);
        unsigned long min_per_thread = 25;
        if (length < 2 * min_per_thread) {

            for (; first != last && done_flag; ++first) {
                if (*first == match) {
                    *done_flag = true;
                    return first;
                }
            }
            return last;
        } else {
            Iterator const mid_point = first + length / 2;
            std::future<Iterator> async_result = std::async(parallel_find_async<Iterator, MatchType>, mid_point, last, match, done_flag);

            Iterator const direct_result = parallel_find_async(first, mid_point, match, done_flag);

            return (direct_result == mid_point) ? async_result.get(): direct_result;
        }
   } catch (...) {
        done_flag->store(true);
        throw;
   }
}

int main() {
    std::random_device rd;
    std::vector<int> ints(testSize);
    for (auto &num : ints) {
        num = 1 + rd() % testSize;
    }

    auto startTime = high_resolution_clock::now();
    int ans = -1;
    for(int i=0; i < testSize; i++) {
        if (ints[i] == 1000) {
            std::cout << "Serially Found " << 618534 << " at index " << i << "\n";
            ans = i;
            break;
        }
    }
    auto endTime = high_resolution_clock::now();
    printf("%s: Time: %fms\n\n", "Serial find", duration_cast<duration<double, milli>>(endTime - startTime));

    startTime = high_resolution_clock::now();
    auto pos = parallel_find_pt(ints.begin(), ints.end(), 618534);
    if (pos != ints.end()) {
        std::cout << "Found " << *pos << " at index " << std::distance(ints.begin(), pos) << "\n";
    }
    endTime = high_resolution_clock::now();
    printf("%s: Time: %fms\n\n", "Parallel find", duration_cast<duration<double, milli>>(endTime - startTime));

    startTime = high_resolution_clock::now();
    std::atomic<bool> done_flag;
    pos = parallel_find_async(ints.begin(), ints.end(), 618534, &done_flag);
    if (pos != ints.end()) {
        std::cout << "Found " << *pos << " at index " << std::distance(ints.begin(), pos) << "\n";
    }
    endTime = high_resolution_clock::now();
    printf("%s: Time: %fms\n\n", "Parallel Async", duration_cast<duration<double, milli>>(endTime - startTime));
}