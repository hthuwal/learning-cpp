#include <iostream>
#include <list>
#include <mutex>
#include <thread>

std::list<int> my_list;
std::mutex m;

/**
 * Since same mutex variable is used by threads, access to list will be mutually exclusive
 **/

void add_to_list(int const& x) {
    std::lock_guard<std::mutex> lg(m);
    my_list.push_front(x);
}

void size() {
    std::lock_guard<std::mutex> lg(m);
    int size = my_list.size();
    std::cout << "size of the list is: " << size << std::endl;
}

void run() {
    std::thread t1(add_to_list, 5);
    std::thread t2(add_to_list, 10);
    std::thread t3(size);

    t1.join();
    t2.join();
    t3.join();
    std::thread t4(size);
    t4.join();
}

int main() {
    run();
}