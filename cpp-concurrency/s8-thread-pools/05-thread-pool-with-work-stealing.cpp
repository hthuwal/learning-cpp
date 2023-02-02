#include "thread-safe-queue.h"
using namespace ::std::literals;

// Very simple Thread safe implementation
// We can optimize it via atomic variables
class WorkStealingQueue {
   private:
    typedef function_wrapper data_type;
    std::deque<data_type> q_;
    mutable std::mutex m_;

   public:
    WorkStealingQueue() {}
    WorkStealingQueue(const WorkStealingQueue& other) = delete;
    WorkStealingQueue& operator=(const WorkStealingQueue& other) = delete;

    void Push(data_type data) {
        std::lock_guard<std::mutex> lock(m_);
        q_.push_front(std::move(data));
    }

    bool Empty() const {
        std::lock_guard<std::mutex> lock(m_);
        return q_.empty();
    }

    bool TryPop(data_type& res) {
        std::lock_guard<std::mutex> lock(m_);
        if (q_.empty()) {
            return false;
        }
        res = std::move(q_.front());
        q_.pop_front();
        return true;
    }

    bool TrySteal(data_type& res) {
        std::lock_guard<std::mutex> lock(m_);
        if (q_.empty()) {
            return false;
        }
        res = std::move(q_.back());
        q_.pop_back();
        return true;
    }
};

/**
 * - Submit a job to queue
 * - One of the worker threads pops the queue and performs work.
 * - Get futures for each submitted task, and wait on the futures to yield.
 * - Each task calls a pool.RunPendingTask untill the future its waiting on yields.
 * - Separate work queue per thread
 * - Allow work stealing
 */
class ThreadPoolWithWorkStealing {
    typedef function_wrapper TaskType;

    std::atomic_bool done;
    ThreadSafeQueue<TaskType> global_work_queue;
    std::vector<std::unique_ptr<WorkStealingQueue>> queues;
    std::vector<std::thread> threads;
    Join_Threads joiner;

    static thread_local WorkStealingQueue* local_work_queue;
    static thread_local unsigned local_work_queue_index;

    void WorkerThread(unsigned index) {
        local_work_queue_index = index;
        local_work_queue = queues[local_work_queue_index].get();
        while (!done) {
            RunPendingTask();
        }
    }

    bool PopTaskFromLocalQueue(TaskType& task) {
        return local_work_queue && local_work_queue->TryPop(task);
    }

    bool PopTaskFromPoolQueue(TaskType& task) {
        return global_work_queue.try_pop(task);
    }

    bool StealTaskFromOtherThreadQueue(TaskType& task) {
        for (unsigned i = 0; i < queues.size(); i++) {
            unsigned const index = (local_work_queue_index + i + 1) % queues.size();
            if (queues[index]->TrySteal(task)) {
                return true;
            }
        }
        return false;
    }

   public:
    ThreadPoolWithWorkStealing() : done(false), joiner(threads) {
        int const thread_count = std::thread::hardware_concurrency();

        try {
            for (int i = 0; i < thread_count; ++i) {
                queues.push_back(std::unique_ptr<WorkStealingQueue>(new WorkStealingQueue));
                threads.push_back(
                    std::thread(&ThreadPoolWithWorkStealing::WorkerThread, this, i));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    ~ThreadPoolWithWorkStealing() {
        done = true;
    }

    template <typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
    submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type;

        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());

        // This will be non empty for pool thread
        if (local_work_queue != nullptr) {
            local_work_queue->Push(std::move(task));
        } else {
            global_work_queue.push(std::move(task));
        }
        return res;
    }

    void RunPendingTask() {
        function_wrapper task;
        if (PopTaskFromLocalQueue(task) || PopTaskFromPoolQueue(task) || StealTaskFromOtherThreadQueue(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
};

thread_local WorkStealingQueue* ThreadPoolWithWorkStealing::local_work_queue = nullptr;
thread_local unsigned ThreadPoolWithWorkStealing::local_work_queue_index = -1;

// parallel quicksort
template <typename T>
struct sorter {
    ThreadPoolWithWorkStealing pool;

    std::list<T> do_sort(std::list<T>& chunk_data) {
        if (chunk_data.size() < 2) {
            return chunk_data;
        }

        std::list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin());

        T const& partition_val = *result.begin();
        typename std::list<T>::iterator divide_point = std::partition(chunk_data.begin(), chunk_data.end(), [&](T const& val) {
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
            pool.RunPendingTask();
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
    const int size = 50;
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