#include <cmath>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <thread_guard.h>

#define MIN_BLOCK_SIZE 1000

void sequential_accumulate(std::vector<long> &nums) {
    long sum = std::accumulate(nums.begin(), nums.end(), 0);
    long prod = std::accumulate(nums.begin(), nums.end(), 1, std::multiplies<long>());

    auto dash_fold = [](std::string a, long b) {
        return std::move(a) + "-" + std::to_string(b);
    };

    auto triangle = [](std::string a, long b) {
        std::string row = "";
        for (long i = 0; i < b; i++) {
            row += "*";
        }
        return std::move(a) + "\n" + std::move(row);
    };

    auto str = std::accumulate(std::next(nums.begin()), nums.end(), std::to_string(nums[0]), dash_fold);
    auto seprinsky = std::accumulate(nums.begin(), nums.end(), std::string(""), triangle);
    printf("Sum: %d\nProduct: %d\nDash Fold: %s\n", sum, prod, str.c_str());
    printf("%s", seprinsky.c_str());
}

template <typename iterator, typename T>
void accumulate(iterator start, iterator end, T &out) {
    out = std::accumulate(start, end, 0);
}

template <typename iterator, typename T>
T parallel_accumulate(iterator start, iterator end, T &init) {
    long input_size = std::distance(start, end);
    long allowed_thread_by_elements = (long)ceil((input_size * 1.0) / MIN_BLOCK_SIZE);
    long allowed_thread_by_hardware = std::thread::hardware_concurrency();

    // No need to spawn threads if there are number of elements is already less than 
    // the block size
    long num_threads = std::min(allowed_thread_by_elements, allowed_thread_by_hardware);
    if (num_threads == 0) {
        return std::accumulate(start, end, init);
    }

    long block_size = (input_size) / num_threads;
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads);

    printf("\nSpawning %d threads each handling %d items out of %d items.\n", num_threads, block_size, input_size);
    iterator last = start;
    for (long i = 0; i < num_threads; i++) {
        std::advance(last, block_size);
        threads[i] = std::thread(accumulate<iterator, T>, start, last, std::ref(results[i]));
        start = last;
    }

    for(auto &thread: threads) {
        thread.join();
    }
    
    return std::accumulate(results.begin(), results.end(), 0);
}

void run() {
    std::vector<long> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    sequential_accumulate(v);
    std::cout << "\n\n";

    long n = 1500000;
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
    long init = 0;
    long psum = parallel_accumulate<std::vector<long>::iterator, long>(nums.begin(), nums.end(), init);
    std::cout << "Parallel Sum: " << psum << "\n";
    et = std::chrono::system_clock::now();
    dur = std::chrono::duration_cast<std::chrono::microseconds>(et - st);
    std::cout << "Took " << dur.count() << " micro seconds\n";
}

int main() {
    run();
}