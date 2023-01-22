#include "common.h"

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x() {
    x.store(true, std::memory_order_seq_cst);
}

void write_y() {
    y.store(true, std::memory_order_seq_cst);
}

void read_x_then_y() {
    // loop until x is true
    while (!x.load(std::memory_order_seq_cst))
        ;

    // check whether y is true
    if (y.load(std::memory_order_seq_cst)) {
        z++;
    }
}

void read_y_then_x() {
    // loop until y is true
    while (!y.load(std::memory_order_seq_cst))
        ;

    // check whether x is true
    if (x.load(std::memory_order_seq_cst)) {
        z++;
    }
}

/**
 * This code can run in two different ways
 *
 * 1. `read_x_then_y` runs first:
 *    - It will wait until x is set to true and then check if y is true.
 *    - Let's imagine at this point y is not set to true, i.e write_y did not get any cycles yet, in that case z won't be incremented.
 *    - Now `read_y_then_x` runs:
 *         - It waits until y is set to true.
 *         - x is already true so it increments the z by 1
 *
 * 2. Similar to 1 but other way around.
 *
 * Because of the sequential consist memoyr order, chnages by one thread are global and visible to each other immediately.
 * The assertions will alway be true.
 **/
void run_code() {
    x = false;
    y = false;
    z = 0;

    {
        std::thread a(write_x);
        std::thread b(write_y);
        std::thread c(read_x_then_y);
        std::thread d(read_y_then_x);

        a.join();
        b.join();
        c.join();
        d.join();
    }
    assert(z != 0);
}

int main() {
    run_code();
}