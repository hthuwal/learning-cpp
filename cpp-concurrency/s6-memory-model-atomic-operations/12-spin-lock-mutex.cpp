#include "common.h"

class Spin_Lock_Mutex {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

   public:
    Spin_Lock_Mutex() {}

    void lock() {
        // If flag is false
        // we'll get false, but the flag will be set to true.
        // we'll exit.
        //
        // If flag is true,
        // This thread will perpetually wait unitll released by someone.

        // We are spinning here indefinitely
        // Using aquire and release memory model for synchronization
        while (flag.test_and_set(std::memory_order_acquire))
            ;
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

Spin_Lock_Mutex mutex;

void func() {
    std::lock_guard<Spin_Lock_Mutex> lg(mutex);
    std::cout << std::this_thread::get_id() << ": hello." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void run_code() {
    std::thread t1(func);
    std::thread t2(func);

    t1.join();
    t2.join();
}

int main() {
    run_code();
}