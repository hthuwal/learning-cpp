#include<thread>
#include<iostream>
#include<mutex>
#include<thread_guard.h>

std::mutex m1;
std::mutex m2;

/**
 * Unique locks deferred mode, lock is not acquired on construction of unique lock
 * Then you acquire all locks together using std::lock
 **/

void func1() {
    printf("Thread1 creating unique_lock for m1 in deferred lock mode..\n");
    std::unique_lock<std::mutex> ul1(m1, std::defer_lock);
    printf("Sleeping for a second.\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("Thread1 creating unique_lock for m2 in deferred lock mode..\n");
    std::unique_lock<std::mutex> ul2(m2, std::defer_lock);
    printf("Thread1 acquiring both locks together.\n");
    std::lock(ul1, ul2);
    printf("Thread1 acquired both locks together.\n");
}

void func2() {
    printf("Thread2 creating unique_lock for m2 in deferred lock mode..\n");
    std::unique_lock<std::mutex> ul2(m2, std::defer_lock);
    printf("Sleeping for a second.\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("Thread2 creating unique_lock for m1 in deferred lock mode..\n");
    std::unique_lock<std::mutex> ul1(m1, std::defer_lock);
    printf("Thread2 acquiring both locks together.\n");
    std::lock(ul1, ul2);
    printf("Thread2 acquired both locks together.\n");
}

void run() {
    std::thread t1(func1);
    Thread_Guard tg1(t1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::thread t2(func2);
    Thread_Guard tg2(t2);
}

int main() {
    run();
}