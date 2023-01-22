#include "common.h"

std::atomic<bool> x, y;
std::atomic<int> z;

/**
 * write_x is synchronized with read_x_then_y (because it reads x first) so it will see whatever writes happen in write_x till release.
 * write_y is synchronized with read_y_then_x only (because it reads y first) so it will see whatever writes happen in write_y till release.
 *
 * assertion may still fail becuase `read_x_then_y` may not get y store, and `read_y_then_x` may not get x store
 */

void write_x() {
    // release is synchronized with acquire
    x.store(true, std::memory_order_release);
}

void write_y() {
    // release is synchronized with acquire
    y.store(true, std::memory_order_release);
}

void read_x_then_y() {
    // loop until x is true
    while (!x.load(std::memory_order_acquire))
        ;

    // check whether y is true
    if (y.load(std::memory_order_acquire)) {
        z++;
    }
}

void read_y_then_x() {
    // loop until y is true
    while (!y.load(std::memory_order_acquire))
        ;

    // check whether x is true
    if (x.load(std::memory_order_acquire)) {
        z++;
    }
}

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