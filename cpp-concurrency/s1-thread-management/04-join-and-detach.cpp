#include <iostream>
#include <thread>

void foo() {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    printf("Hello from foo. \n");
}

void bar() {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    printf("Hello from bar. \n");
}

void run() {
    std::thread foo_thread(foo);
    std::thread bar_thread(bar);
    bar_thread.detach();
    printf("After bar detach and before the foo join\n");
    foo_thread.join();
    printf("After foo join.\n");
}

int main() {
    run();
}