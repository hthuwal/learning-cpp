#include<iostream>
#include<thread>
using namespace std;

void test() {
    printf("Hello from test.\n");
}

void check_joinable(std::thread &t, std::string name) {
    if (t.joinable()) {
        printf("%s is joinable.\n", name.c_str());
    } else {
        printf("%s is not joinable.\n", name.c_str());
    }
}

void run() {
    std::thread thread1(test);
    check_joinable(thread1, "thread1");
    thread1.join();
    // No longer joinable after calling join
    check_joinable(thread1, "thread1");

    // Not a proper thred, no callable so not joinable.
    std::thread thread2;
    check_joinable(thread2, "thread2");
}

int main() {
    run();
}