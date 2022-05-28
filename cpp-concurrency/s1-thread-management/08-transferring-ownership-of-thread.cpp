#include<iostream>
#include<thread>

void foo() {
    printf("Hello from foo.\n");
}

void bar() {
    printf("Hello from bar.\n");
}

void run() {
    std::thread thread_1(foo);

    //std::thread thread_2 = thread_1; Will give error as Thread does not have copy constructor.
    std::thread thread_2 = std::move(thread_1); 
    // We transferred the ownership to thread_2 variable.
    // thread_1 variable does not own any thread object

    // r value assignment, move constructor is called;
    thread_1 = std::thread(bar); 

    // transfering ownership to a variable that already owns another thread object
    std::thread thread_3(foo);
    // thread_1 = std::move(thread_3);
    // Cannot transfer ownership

    thread_1.join();
    thread_2.join();
    thread_3.join();
}

int main() {
    run();
}