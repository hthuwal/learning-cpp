#include <atomic>

#include "common.h"

void run_code() {
    std::atomic<bool> flag1;  // false by default
    std::cout << "flag 1 = " << flag1 << std::endl;

    // Cannot copy construct: it is a deleted function
    // std::atomic<bool> flag2 (flag1);

    // Cannot copy assign
    // std::atomic<bool> flag2 = flag1;

    // Construct using non atomic boolean value
    bool non_atomic_flag = true;
    std::atomic<bool> flag4(non_atomic_flag);
    std::cout << "flag 4 = " << flag4 << std::endl;

    // assigning non atomic boolean value
    std::atomic<bool> flag5 = non_atomic_flag;
    std::cout << "flag 5 = " << flag5 << std::endl;
}

void run_code_operations() {
    std::atomic<bool> x(false);

    std::cout << "atomic boolean is implemented lock free - " << (x.is_lock_free() ? "yes" : "no") << std::endl;

    std::atomic<bool> y(true);

    // store operations
    x.store(false);
    x.store(y);  // store another atomic variable

    // load operations
    std::cout << "value of atomic bool y is - " << y.load() << std::endl;

    // exchange operation

    bool previous_value = x.exchange(false);
    std::cout << "previous value of atomic bool x is - " << previous_value << std::endl;
    std::cout << "current value of atomic bool x is - " << x.load() << std::endl;
}

int main() {
    run_code_operations();
}