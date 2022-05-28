#include<thread>
#include<iostream>
#include<mutex>
#include<thread_guard.h>

std::mutex m1;
std::mutex m2;

/**
 * There's a deadlock in the program because func1 acquires lock1 and then wants lock2
 * while func2 acquires lock2 and then wants lock1.
 **/

void func1() {
    printf("Thread1 waiting to acquire lock m1.\n");
    std::lock_guard<std::mutex> lg1(m1);
    printf("Thread1 acquired lock m1.\n");
    printf("Sleeping for a second.\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("Thread1 waiting to acquire lock m2.\n");
    std::lock_guard<std::mutex> lg2(m2);
    printf("Thread1 acquired lock m2.\n");
}

void func2() {
    printf("Thread2 waiting to acquire lock m2.\n");
    std::lock_guard<std::mutex> lg2(m2);
    printf("Thread2 acquired lock m2.\n");
    printf("Sleeping for a second.\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("Thread2 waiting to acquire lock m1.\n");
    std::lock_guard<std::mutex> lg1(m1);
    printf("Thread2 acquired lock m1.\n");
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