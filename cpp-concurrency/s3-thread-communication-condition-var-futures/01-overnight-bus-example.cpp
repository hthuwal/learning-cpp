#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "thread_guard.h"

bool have_i_arrived = false;
int distance_to_destination = 10;
int distance_covered = 0;

// Represents bus driver driving.
bool keep_driving() {
    while (distance_covered < distance_to_destination) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        distance_covered++;
    }
    return false;
}

// Represents you staying awake and keep checking whether you're there or not.
void keep_awake_all_night() {
    while (distance_covered < distance_to_destination) {
        std::cout << "Checking If I am there." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::cout << "Finally I am there, distance_covered = " << distance_covered << "/" << distance_to_destination << std::endl;
}

void set_alarm_and_take_a_nap() {
    if (distance_covered < distance_to_destination) {
        std::cout << "Let me take a nap" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(distance_to_destination * 1000));
    }
    std::cout << "Finally I am there after the nap, distance_covered = " << distance_covered << "/" << distance_to_destination << std::endl;
}

void run_code() {
    std::thread driver(keep_driving);
    Thread_Guard tgd(driver);
    std::thread keep_awake_all_night_thread(keep_awake_all_night);
    Thread_Guard tgk(keep_awake_all_night_thread);
    std::thread set_alarm_and_take_a_nap_thread(set_alarm_and_take_a_nap);
    Thread_Guard tgs(set_alarm_and_take_a_nap_thread);
}

/**
 * A better solution is to ask someone to wake us up. Using condition variable.
 **/

int main() {
    run_code();
}