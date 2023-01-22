#include "common.h"

std::atomic<int> data[5];
std::atomic<bool> sync1(false), sync2(false);

void func1() {
    for (int i = 0; i < 5; i++) {
        data[i].store((i + 1) * 5, std::memory_order_relaxed);
    }
    // Synchronizes func1 to with func2
    sync1.store(true, std::memory_order_release);
}

void func2() {
    while (!sync1.load(std::memory_order_acquire))
        ;
    // synchronizes func2 to with func3
    sync2.store(true, std::memory_order_release);
}

void func3() {
    while (!sync2.load(std::memory_order_acquire))
        ;
    for (int i = 0; i < 5; i++) {
        assert(data[i].load(std::memory_order_relaxed) == (i + 1) * 5);
    }
}

void run_code() {
    {
        std::thread thread_1(func1);
        std::thread thread_2(func2);
        std::thread thread_3(func3);

        thread_1.join();
        thread_2.join();
        thread_3.join();
    }
}

int main() {
    run_code();
}