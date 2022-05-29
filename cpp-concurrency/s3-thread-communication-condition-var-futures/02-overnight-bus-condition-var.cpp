#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "thread_guard.h"

bool have_i_arrived = false;
int total_distance = 10;
int distance_covered = 0;

std::condition_variable cv;
std::mutex m;

void keep_moving() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        distance_covered++;

        // Notify threds who are waiting
        if (distance_covered == total_distance) {
            cv.notify_one();
        }
    }
}

void ask_drivier_to_wake_u_up_at_right_time() {
    std::unique_lock<std::mutex> ul(m);
    // When thread reaches here, 
    // - it will take ownership of the mutex and check the expression.
    // - If condition is not true, cv will unlock the associated mutex and make the thread sleep untill it gets notified.
    // - When notify is called, the cv will take the lock and evaluate again. Expression now is true and thread will move on to next line while holding the lock.
    cv.wait(ul, [] { return distance_covered == total_distance; });
    std::cout << "Driver woke me up, I am there. Distance Covered = " << distance_covered << "/" << total_distance << std::endl;
}

void run_code() {
    std::thread driver_thread(keep_moving);
    Thread_Guard tgd(driver_thread);
    std::thread passenger_thread(ask_drivier_to_wake_u_up_at_right_time);
    Thread_Guard tgp(passenger_thread);
}