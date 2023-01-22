#include <atomic>

// How to use memory orders
void run_code() {
    std::atomic<int> x;

    x.store(5);

    x.store(10, std::memory_order_release);
    x.load(std::memory_order_acquire);

    int value = 11;

    // One for success and one for match failure.
    bool ret_val = x.compare_exchange_weak(value, 13, std::memory_order_release, std::memory_order_relaxed);
}