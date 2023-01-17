// Need to compile it with -ltbb

#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <random>
#include <vector>

using std::milli;
using std::random_device;
using std::sort;
using std::vector;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

const size_t testSize = 6'000'000;
const int iterationCount = 5;

void print_results(const std::string& title, const std::vector<double>& sorted, const high_resolution_clock::time_point& start_time, const high_resolution_clock::time_point& end_time) {
    printf("%s: Lowest: %g Highest %g Time:%fms\n", title.c_str(), sorted.front(), sorted.back(), duration_cast<duration<double, milli>>(end_time - start_time));
}

int main() {
    random_device rd;

    printf("Testing with %zu doubles... \n", testSize);
    vector<double> doubles(testSize);
    for (auto& d : doubles) {
        d = static_cast<double>(rd());
    }

    for (int i = 0; i < iterationCount; i++) {
        vector<double> sorted(doubles);
        const auto startTime = high_resolution_clock::now();
        sort(sorted.begin(), sorted.end());
        const auto endTime = high_resolution_clock::now();
        print_results("Serial STL", sorted, startTime, endTime);
    }

    for (int i = 0; i < iterationCount; i++) {
        vector<double> sorted(doubles);
        const auto startTime = high_resolution_clock::now();
        sort(std::execution::par, sorted.begin(), sorted.end());
        const auto endTime = high_resolution_clock::now();
        print_results("Parallel STL", sorted, startTime, endTime);
    }
}