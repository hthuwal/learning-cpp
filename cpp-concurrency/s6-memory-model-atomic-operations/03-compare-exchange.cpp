#include <atomic>

#include "common.h"

void run_code() {
    std::atomic<int> x(20);

    int expected_value = 20;
    std::cout << "Expected value variable holds: " << expected_value << std::endl;

    std::cout << "Calling x.compare_exchange_weak(expected_value, 6)" << std::endl;
    bool return_val = x.compare_exchange_weak(expected_value, 6);

    std::cout << "Returned Value, operation successful : " << (return_val ? "yes" : "no") << std::endl;
    std::cout << "Expected Value variable holds        : " << expected_value << std::endl;
    std::cout << "Current value of atomic variable x   : " << x.load() << std::endl
              << std::endl;

    expected_value = 25;
    x = 20;
    std::cout << "Expected value variable holds: " << expected_value << std::endl;

    std::cout << "Calling x.compare_exchange_weak(expected_value, 6)" << std::endl;
    return_val = x.compare_exchange_weak(expected_value, 6);

    std::cout << "Returned Value, operation successful : " << (return_val ? "yes" : "no") << std::endl;
    std::cout << "Expected Value variable holds        : " << expected_value << std::endl;
    std::cout << "Current value of atomic variable x   : " << x.load() << std::endl;
}

int main() {
    run_code();
}