#include <barrier>

#include "common.h"

std::barrier barr(3, []() noexcept {
    // locking not needed here
    std::cout << "Barrier Limit reached Opening!\n";
});

void func(std::string name) {
    // Wait until the barrier is open
    std::cout << name << " entered and waiting first\n";
    barr.arrive_and_wait();
    std::cout << name << " barrier opened once, waitin for it to open again\n";

    // Wait until the barrier is Open again
    barr.arrive_and_wait();
    std::cout << name << " barrier opened agin\n";
}

int main() {
    std::vector<std::string> names = {"Harish", "Chandra", "Thuwal"};

    std::thread th1(func, names[0]);
    std::thread th2(func, names[1]);
    std::thread th3(func, names[2]);

    th1.join();
    th2.join();
    th3.join();
}
