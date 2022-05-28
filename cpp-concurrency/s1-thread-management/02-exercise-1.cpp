#include <iostream>
#include <thread>

void test() {
    printf("Hello From Test.\n");
}

void funcA() {
    printf("Hello From funcA.\n");
    std::thread threadC(test);
    threadC.join();
}

void funcB() {
    printf("Hello From funcB.\n");
}

void run() {
    std::thread threadA(funcA);
    std::thread threadB(funcB);

    threadA.join();
    threadB.join();
    printf("Hello from main thread.");
}

int main() {
    run();
}