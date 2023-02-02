#include "thread-safe-queue.h"
using namespace ::std::literals;

/**
 * Simple Thread
 * - Submit a job to queue
 * - One of the worker threads pops the queue and performs work.
 * - Get futures for each submitted task, and wait on the futures to yield.
 *
 * Lacks:
 * - All threads may end up waiting for the tasks that haven't been scheduled yet. (Think recursive merge sort)
 * - We have no way of forcing for waiting threads to execute another tasks untill the results of waiting task arrives.
 * - We have no way of switching tasks when they are executing.
 */
class thread_pool_waiting {
    std::atomic_bool done;
    ThreadSafeQueue<function_wrapper> work_queue;
    std::vector<std::thread> threads;
    Join_Threads joiner;

    void worker_thread() {
        while (!done) {
            function_wrapper task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

   public:
    thread_pool_waiting() : done(false), joiner(threads) {
        int const thread_count = std::thread::hardware_concurrency();

        try {
            for (int i = 0; i < thread_count; ++i) {
                threads.push_back(
                    std::thread(&thread_pool_waiting::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    ~thread_pool_waiting() {
        done = true;
    }

    template <typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
    submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push(std::move(task));
        return res;
    }
};

int main() {
    thread_pool_waiting pool;
    std::cout << "Testing thread pool" << std::endl;
    std::vector<std::future<int>> futures;

    for (int i = 0; i < 100; i++) {
        futures.emplace_back(pool.submit([i]() -> int {
            printf("%d printed by thread - %lld\n", i, std::this_thread::get_id());
            // std::this_thread::sleep_for(1s);
            return i;
        }));
    }

    for (auto& fut : futures) {
        printf("Got response: %d\n", fut.get());
    }
}