#include <iostream>
#include <list>
#include <algorithm>
#include <future>
#include <random>

const size_t testSize = 100000;
const int iterationCount = 5;

void print_results(const std::string& title, const std::list<double>& sorted, const std::chrono::high_resolution_clock::time_point& start_time, const std::chrono::high_resolution_clock::time_point& end_time) {
    // for(const auto& each: sorted) {
    //     std::cout<<each<<" ";
    // };
    // std::cout<<std::endl;
    printf("%s: Lowest: %g Highest %g Time:%fms\n", title.c_str(), sorted.front(), sorted.back(), std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end_time - start_time));
}

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input) {
    // recursive call limit condition
    if (input.size() < 2) {
        return input;
    }
    
    // select the pivot value
    std::list<T> results;
    results.splice(results.begin(), input, input.begin()); // moves the first element from input to results
    T pivot = *results.begin();

    // rearrange elements
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t) {
        return t < pivot;
    });


    // recursive call
    std::list<T> lower_list;
    lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

    auto new_lower(sequential_quick_sort(std::move(lower_list)));
    auto new_upper(sequential_quick_sort(std::move(input)));

    // arrange the results of recursive call
    results.splice(results.begin(), new_lower);
    results.splice(results.end(), new_upper);

    return results;
}

// In parctice performs very poorly because there's no limit on number of threds?
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    // recursive call limit condition
    if (input.size() < 2) {
        return input;
    }
    
    // select the pivot value
    std::list<T> results;
    results.splice(results.begin(), input, input.begin()); // moves the first element from input to results
    T pivot = *results.begin();

    // rearrange elements
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t) {
        return t < pivot;
    });


    // recursive call
    std::list<T> lower_list;
    lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

    auto new_lower(parallel_quick_sort(std::move(lower_list)));
    std::future<std::list<T>> new_upper_future = std::async(&parallel_quick_sort<T>, std::move(input));

    // arrange the results of recursive call
    results.splice(results.begin(), new_lower);
    results.splice(results.end(), new_upper_future.get());

    return results;
}

int main() {
    std::random_device rd;

    printf("Testing with %zu doubles... \n", testSize);
    std::list<double> doubles(testSize);
    for (auto& d : doubles) {
        d = static_cast<double>(rd());
    }

    for (int i = 0; i < iterationCount; i++) {
        const auto startTime = std::chrono::high_resolution_clock::now();
        const auto sorted = sequential_quick_sort(doubles);
        const auto endTime = std::chrono::high_resolution_clock::now();
        print_results("Serial Sort", sorted, startTime, endTime);
    }

     for (int i = 0; i < iterationCount; i++) {
        const auto startTime = std::chrono::high_resolution_clock::now();
        const auto sorted = parallel_quick_sort(doubles);
        const auto endTime = std::chrono::high_resolution_clock::now();
        print_results("Parallel Sort", sorted, startTime, endTime);
    }
}