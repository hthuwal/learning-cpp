#include "thread-safe-queue.h"
using namespace ::std::literals;

/**
 * Simple Thread
 * - Submit a job to queue
 * - One of the worker threads pops the queue and performs work.
 * - Get futures for each submitted task, and wait on the futures to yield.
 * - Each task calls a pool.run_pending_tasks untill the future its waiting on yields.
 *
 * Problem
 * - When the number of processes increase, their is increasing contention on the queue. In this type of scenario, cache ping pong can be substantial time sink.
 */
class thread_pool_waiting_other_tasks {
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
    thread_pool_waiting_other_tasks() : done(false), joiner(threads) {
        int const thread_count = std::thread::hardware_concurrency();

        try {
            for (int i = 0; i < thread_count; ++i) {
                threads.push_back(
                    std::thread(&thread_pool_waiting_other_tasks::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    ~thread_pool_waiting_other_tasks() {
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

    void run_pending_tasks() {
        function_wrapper task;
        if (work_queue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
};

// parallel quicksort
template <typename T>
struct sorter {
    thread_pool_waiting_other_tasks pool;

    std::list<T> do_sort(std::list<T> &chunk_data) {
        if (chunk_data.size() < 2) {
            return chunk_data;
        }

        std::list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin());

        T const &partition_val = *result.begin();
        typename std::list<T>::iterator divide_point = std::partition(chunk_data.begin(), chunk_data.end(), [&](T const &val) {
            return val < partition_val;
        });

        std::list<T> new_lower_chunk;
        new_lower_chunk.splice(new_lower_chunk.end(), chunk_data, chunk_data.begin(), divide_point);

        std::future<std::list<T>> sorted_lower = pool.submit(std::bind(&sorter::do_sort, this, std::move(new_lower_chunk)));
        std::list<T> new_higher(do_sort(chunk_data));

        result.splice(result.end(), new_higher);

        /**
         * Following piece of code checks if future is not ready
         * If its not ready it yields another task from the worker queue
         *
         * Without this it may happen all thread end up waiting indifinetely at line 105
         * While the tasks being waited on never get at thread to run on.
         *
         * Try commenting out and see that the code hangs
         */
        while (sorted_lower.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
            // while (new_lower._Is_ready()) // Not present in compiler?
            pool.run_pending_tasks();
        }

        result.splice(result.begin(), sorted_lower.get());
        return result;
    }
};

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }

    sorter<T> s;
    return s.do_sort(input);
}

void run() {
    const int size = 800;
    std::list<int> my_list;

    srand(0);
    for (int i = 0; i < size; i++) {
        my_list.push_back(rand() % 1000);
    }

    my_list = parallel_quick_sort(my_list);

    for (int i = 0; i < size; i++) {
        std::cout << my_list.front() << " ";
        my_list.pop_front();
    }
}

int main() {
    run();
}