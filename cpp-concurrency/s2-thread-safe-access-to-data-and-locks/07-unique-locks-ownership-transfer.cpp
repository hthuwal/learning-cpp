#include<thread>
#include<string>
#include<iostream>
#include<mutex>

// By transferring the ownership of unique locks we can ensure
// that different places use the same lock

std::mutex m;


void x_op() {
    std::cout<< "This is X operation." << std::endl;
}

void y_op() {
    std::cout<< "This is Y operation." << std::endl;
}

std::unique_lock<std::mutex> get_unique_lock() {
    std::unique_lock<std::mutex> lk(m);
    x_op();
    return lk;
}

int main() {
    std::unique_lock<std::mutex> lk_main(get_unique_lock()); //Move ownership of the lock to lk_main
    y_op();
}