#include <atomic>

#include "common.h"

void run_code() {
    // std::atomic_flag flag1 = true;
    std::atomic_flag flag1 = ATOMIC_FLAG_INIT;

    std::cout << "1. Test return value:" << flag1.test() << "\n";
    std::cout << "2. Test and set's return value:" << flag1.test_and_set() << "\n";
    std::cout << "3. Test and set's return value:" << flag1.test_and_set() << "\n";

    std::cout << "4. Clearing flag.\n";
    std::cout << "5. Test and set's return value:" << flag1.test_and_set() << "\n";
}

int main() {
    run_code();
}