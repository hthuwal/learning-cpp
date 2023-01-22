#include "common.h"

std::atomic<bool> x, y;
std::atomic<int> z;

/**
 * If we had used memory_order_seq_cst
 *
 * Since store of x happens before store of y. And read of y happens before read of x.
 * We could gaurantee that x is written to before being read from.
 *
 * But since we used `memory_order_relaxed` we can't gaurantee a global view of variables between threads.
 * And hence cannot gaurantee that x is written to before being read from.
 *
 * It could happen that only the flush to x by write thread is propogated to view of read thread.
 */

/**
 * From writer threads pov, write to x happens before write to y
 */
void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}

/**
 * From reads thread pov, load of y happens before load of x.
 */
void read_y_then_x() {
    // loop until y is true
    while (!y.load(std::memory_order_seq_cst))
        ;

    // check whether x is true
    if (x.load(std::memory_order_seq_cst)) {
        z++;
    }
}

void run_code() {
    x = false;
    y = false;
    z = 0;

    {
        std::thread c(write_x_then_y);
        std::thread d(read_y_then_x);

        c.join();
        d.join();
    }
    assert(z != 0);
}

int main() {
    run_code();
}