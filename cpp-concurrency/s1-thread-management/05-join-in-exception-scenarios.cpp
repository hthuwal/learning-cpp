#include <iostream>
#include <thread>
#include "thread_guard.h"

using namespace std;

void foo() {
    printf("Hello from foo. \n");
}

void other_operations() {
    std::cout << "This is other operation\n";
    throw std::runtime_error("this is a runtime error");
}



void run() {

    // Solution 1 to ensure gauranteed join function call
    // Not great solution. Can't use try catch everywhere in the code.
    std::thread foo_thread(foo);
    try {
        other_operations();
        foo_thread.join();
    } catch (...) {
        foo_thread.join();
    }

    // Solution 2 use RAII principle. Use the thread_gurad class
    // Destructor for the classes are called in the reverse order of their creation
    // So when the code finishes, terminate of foo_thread2 will not be called.
    // Because the destructor of thread_guard will be called, which will join foo_thread2
    // Main thread does not have to explicity join the thread.
    std::thread foo_thread2(foo);
    Thread_Guard tg(foo_thread2);
    try {
        other_operations();
    } catch(...) {

    }
}

int main() {
    run();
}