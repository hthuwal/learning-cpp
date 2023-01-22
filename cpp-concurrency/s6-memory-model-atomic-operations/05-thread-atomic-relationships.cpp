#include "common.h"

std::atomic<bool> data_ready = false;
std::vector<int> data_vector;

void reader_func() {
    // Has to execute this while loop.
    // Happen before actually accessing data: Happen Before Relationship.

    // We use data_ready to prevent read before write: Synchronize With relationship.
    // Write and read happening in different threads so it's: Inter thread relationship.
    while (!data_ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << data_vector[0] << std::endl;
}

void writer_func() {
    data_vector.push_back(3);
    data_ready.store(true);
}

void run_code() {
    std::thread reader_thread(reader_func);
    std::thread writer_thread(writer_func);

    reader_thread.join();
    writer_thread.join();
}

int main() {
    run_code();
}