#include "common.h"

std::vector<int> getNumbers(int start, int end, int inc = 1) {
    std::vector<int> nums;
    for (int i = start; i < end; i += inc) {
        nums.push_back(i);
    }
    return std::move(nums);
}

int main() {
    const auto numbers = getNumbers(10, 30);

    for (int i = 0; i < 10; i++) {
        std::cout << numbers[i] << " ";
    }

    std::cout << "\n";

    for (int i = 10; i < 20; i++) {
        std::cout << numbers[i] << " ";
    }
}