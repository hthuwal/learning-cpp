#include "thread-safe-queue.h"
using namespace ::std::literals;

/**
 * Simple Thread
 * - Submit a job to queue
 * - One of the worker threads pops the queue and performs work.
 *
 * Lacks:
 * - Ability to wait on tasks.
 * - Do not have capability to get responses from threads
 */

class ThreadPool {
    std::atomic_bool done;

    // Queue of Callable objects
    ThreadSafeQueue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;

    Join_Threads joiner;

    // Each thread will pop queue for work and perform it
    void worker_thread() {
        while (!done) {
            std::function<void()> task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                // yield cpu because there's nothing to do
                std::this_thread::yield();
            }
        }
    }

   public:
    // When thread pool is created spawn threads
    ThreadPool() : done(false), joiner(threads) {
        int const thread_count = std::thread::hardware_concurrency();

        try {
            for (int i = 0; i < thread_count; i++) {
                threads.push_back(std::thread(&ThreadPool::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    // When thread pool is destroyed, signal other threads to exit
    ~ThreadPool() {
        done = true;
    }

    template <typename Function_type>
    void submit(Function_type f) {
        work_queue.push(std::function<void()>(f));
    }
};

int main() {
    ThreadPool pool;
    std::cout << "Testing thread pool" << std::endl;
    for (int i = 0; i < 100; i++) {
        pool.submit([i] {
            printf("%d printed by thread - %lld\n", i, std::this_thread::get_id());
        });
    }

    std::this_thread::sleep_for(2s);
}