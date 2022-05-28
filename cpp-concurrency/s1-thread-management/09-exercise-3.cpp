/*
exercise-2.cpp

Two engine work queue and clean work queue
Engine and clean crew are represent by one thread which read from the queue.
If there is a command they perform work and sleep for 1 second.
If there is no command sleep for 2 second
Main thread would add command to queue.

These thread will also check for a flag is Done and exit if that is true.
Main thread set flag to true when user issues 100.

Captain does not have to wait for any command to finish and must wait for 1 second between
two commands.
*/

#include <iostream>
#include <queue>
#include <thread>
#include <thread_guard.h>

void process_command(int cmd) {
    if (cmd == 1) {
        printf("Cleaning.\n");
    } else if (cmd == 2) {
        printf("Full sleep ahead.\n");
    } else if (cmd == 3) {
        printf("Stop the engine.\n");
    } else {
        printf("Invalid Command.\n");
    }
    printf("\nEnter Command: "); fflush(stdout);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("\nExecution Done. Enter Command: "); fflush(stdout);
}

void work(std::queue<int>& work_queue, bool& is_done) {
    auto complete = [&work_queue, &is_done]() {
        return is_done && work_queue.empty();
    };

    while (!complete()) {
        if (!work_queue.empty()) {

            int work_cmd = work_queue.front();
            work_queue.pop();
            process_command(work_cmd);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }
}

void run() {
    int command;
    std::queue<int> engine_cmd_queue;
    std::queue<int> clean_cmd_queue;
    bool is_done = false;

    std::thread engine_work_thread(work, std::ref(engine_cmd_queue), std::ref(is_done));
    Thread_Guard tge(engine_work_thread);

    std::thread cleaner_thread(work, std::ref(clean_cmd_queue), std::ref(is_done));
    Thread_Guard tgc(cleaner_thread);    

    printf("\nEnter Command: ");
    fflush(stdout);
    while (std::cin >> command) {
        if (command == 100) {
            is_done = true;
            break;
        } else if (command == 1) {
            clean_cmd_queue.push(command);
        } else if (command == 2 || command == 3) {
            engine_cmd_queue.push(command);
        } else {
            std::cout << "Invalid Command"
                      << "\nEnter Command: ";
        }
    }
}

int main() {
    run();
}