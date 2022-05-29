#include <future>
#include <iostream>

int meaning_of_life() {
    return 42;
}

void do_other_calculations() {
    std::cout << "Doing other stuff" << std::endl;
}

void run_code() {
    std::future<int> mol = std::async(meaning_of_life);
    do_other_calculations();
    std::cout << "The meaning of life is " << mol.get() << std::endl;
}

int main() {
    run_code();
}