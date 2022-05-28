#include <iostream>
#include <thread>
#include "thread_guard.h"

void sum_func1(int x, int y) {
    printf("%d + %d = %d\n", x, y, x+y);
}

// Thread callable that takes a reference
void sum_func2(int &x) {
    while(x != 100) {
        printf("Thread 1 value of X is %d\n", x);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void run() {
    std::thread sum_thread(sum_func1, 4, 5);
    Thread_Guard tg(sum_thread);

    int x = 9;
    printf("Main thread value of X is %d\n", x);

    // thread constructor takes its arguments by value, explicitly make it ref
    // thread arguments are by default pass by value, you have to explicitly say that its a ref
    // if you want it passed as ref
    std::thread sum_thread_2(sum_func2, std::ref(x));
    Thread_Guard tg2(sum_thread_2);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    x = 15;
    printf("Main thread value of X has been changed to %d\n", x);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    x = 100;
}

int main() {
    run();
}