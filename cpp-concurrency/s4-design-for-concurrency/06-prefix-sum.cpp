#include "common.h"

template <typename Iterator, typename OutIterator>
void sequential_partial_sum(Iterator first, Iterator last, OutIterator ans) {
    unsigned long const length = std::distance(first, last);

    ans[0] = first[0];
    for (int i = 1; i < length; i++) {
        ans[i] = ans[i - 1] + first[i];
    }
}

template <typename Iterator>
void parallel_partial_sum(Iterator first, Iterator last) {
    typedef typename Iterator::value_type value_type;

    struct process_chunk {
        void operator()(Iterator begin, Iterator last, std::future<value_type> *sum_till_previous_chunk, std::promise<value_type> *sum_till_this_chunk) {
            try {
                
                // Calculate prefix sum for this chunk
                Iterator exclusiveLast = last+1;
                std::partial_sum(begin, exclusiveLast, begin);

                // If sum_till_previous_chunk is a valid pointer --> This is not the first chunk
                if (sum_till_previous_chunk) {

                    // Wait for the sum till prev_chunk to come
                    auto previous_block_sum = sum_till_previous_chunk->get();
                    
                    // Add sum till previous check to last of this chunk's prefix sum
                    *last += previous_block_sum;

                    // Update promise till this chunk so that subsequent threads are free
                    if (sum_till_this_chunk) {
                        sum_till_this_chunk->set_value(*last);
                    }

                    // Add sum till previous chunk to rest of the prefix sum of this chunk
                    std::for_each(begin, last, [previous_block_sum](value_type &each) {
                        each += previous_block_sum;
                    });
                } else {
                    // This is the first chunk no need to wait for anythin
                    sum_till_this_chunk -> set_value(*last);   
                }


            } catch (...) {
                if (this_chunk_sum_promise) {
                    this_chunk_sum_promise->set_exception(std::current_exception());
                } else {
                    throw;
                }
            }
        }
    };



    unsigned long const length = std::distance(first, last);

    if (length == 0) {
        return;
    }

    unsigned long const min_per_thread = 25;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const max_threads = length / min_per_thread + (length % min_per_thread == 0 ? 0 : 1);

    unsigned long const num_threads = std::min(hardware_threads, max_threads);
    unsigned long const block_size = length / num_threads;

    std::vector<std::thread> threads(num_threads - 1);
    std::vector<std::promise<value_type>> end_values(num_threads - 1);
    std::vector<std::future<value_type>> prev_end_values;

    Join_Threads joiner(threads);

    prev_end_values.reserve(num_threads - 1);

    Iterator block_start = first;
    for (int i=0 ;i < num_threads-1; i++) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size - 1);

        threads[i] = std::thread(process_chunk(), block_start, block_end, i != 0 ? &prev_end_values[i-1]: 0, &end_values[i]);

        block_start = block_end;
        ++block_start;
        prev_end_values.push_back(end_values[i].get_future());
    }

    Iterator final_element = block_start;
    std::advance(final_element, std::distance(block_start, last) - 1);
    process_chunk()(block_start, final_element, (num_threads > 1) ? &prev_end_values.back(): 0, 0);
}

void print_results(const std::string& title, const std::vector<int>& ints, const high_resolution_clock::time_point& start_time, const high_resolution_clock::time_point& end_time) {
    printf("%s: Time: %fms\n", title.c_str(), duration_cast<duration<double, milli>>(end_time - start_time));
    // printVector(ints);
    // std::cout<<"\n\n";
}

const size_t testSize = 30;

int main() {
    std::random_device rd;
    std::vector<int> ints(testSize);
    for (auto& num : ints) {
        num = 1 + rd() % testSize;
    }
    printVector(ints); std::cout<<"\n";

    {
        std::vector<int> ans(ints.size());
        auto startTime = high_resolution_clock::now();
        sequential_partial_sum(ints.begin(), ints.end(), ans.begin());
        auto endTime = high_resolution_clock::now();
        print_results("Sequential Partial Sum", ans, startTime, endTime);
    }

    // C++17 Inclusive scan: sum[i] = sum[i-1] + a[i]
    {
        std::vector<int> ans(ints.size());
        auto startTime = high_resolution_clock::now();
        std::inclusive_scan(ints.begin(), ints.end(), ans.begin());
        auto endTime = high_resolution_clock::now();
        print_results("Sequential Inclusive Sum", ans, startTime, endTime);
    }

    // C++17 Parallel Inclusive scan
    {
        std::vector<int> ans(ints.size());
        auto startTime = high_resolution_clock::now();
        std::inclusive_scan(std::execution::par, ints.begin(), ints.end(), ans.begin());
        auto endTime = high_resolution_clock::now();
        print_results("Prallel Inclusive Sum", ans, startTime, endTime);
    }

    // Our parallele prefix sum
   {
        std::vector<int> ans(ints);
        auto startTime = high_resolution_clock::now();
        parallel_partial_sum(ans.begin(), ans.end());
        auto endTime = high_resolution_clock::now();
        print_results("Prallel Inclusive Sum", ans, startTime, endTime);
    }


    // Exclusive scan: sum[i] = sum[i-2] + a[i-1]
    // Partial_Sum, same as inclusive but specify the operation instead of just sum
}