#include <atomic>

#include "common.h"

void run_code() {
    int values[20];

    for (int i = 0; i < 20; i++) {
        values[i] = i + 1;
    }

    /* ---------------------------- Common Operation ---------------------------- */

    std::atomic<int*> x_pointer = values;
    std::cout << "Is atomic integer pointer lock free ? " << (x_pointer.is_lock_free() ? "yes" : "no") << std::endl;

    int* y_pointer = values + 3;
    x_pointer.store(y_pointer);

    std::cout << "Atomic integer pointer refers to: " << *(x_pointer.load()) << std::endl;

    bool ret_val = x_pointer.compare_exchange_weak(y_pointer, values + 10);
    std::cout << "Store operation successfull: " << (ret_val ? "yes" : "no") << std::endl;
    std::cout << "Atomic integer pointer now refers to: " << *(x_pointer) << std::endl;

    /* ------------------- Atomic Pointer specific operations ------------------- */
    std::cout << "\n";
    x_pointer = values;

    std::cout << "1. Atomic integer pointer refers to: " << *(x_pointer.load()) << std::endl;

    // Return the prev pointer value and increment the pointer by 12.
    int* prev_pointer_val_1 = x_pointer.fetch_add(12);

    std::cout << "2. Pointer returned by fetch add points to: " << *prev_pointer_val_1 << std::endl;
    std::cout << "2. After fetch add atomic pointer points to: " << *x_pointer << std::endl;

    // Return the prev pointer value and decrement the pointer by 3.
    int* prev_pointer_val_2 = x_pointer.fetch_sub(3);

    std::cout << "3. Pointer returned by fetch sub points to: " << *prev_pointer_val_2 << std::endl;
    std::cout << "3. After fetch sub atomic pointer points to: " << *x_pointer << std::endl;

    // ++
    x_pointer++;
    std::cout << "4. After post increment atomic pointer points to: " << *x_pointer << std::endl;

    // --
    x_pointer--;
    std::cout << "5. After post decrement atomic pointer points to: " << *x_pointer << std::endl;
}

int main() {
    run_code();
}