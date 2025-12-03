#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream inputFile{"input.txt"};
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::uint64_t totalJoltage{0};
    const size_t k{12};

    for (std::string line{}; std::getline(inputFile, line);) {
        int stack[k]{};
        size_t size{0};
        const size_t n{line.size()};

        for (size_t i{0}; i < n; ++i) {
            int currentDigit{line[i] - '0'};
            size_t remaining{n - i - 1};

            while (size && currentDigit > stack[size - 1] && ((size - 1 + remaining + 1) >= k)) {
                --size;
            }

            if (size < k) {
                stack[size++] = currentDigit;
            }
        }

        std::uint64_t number{0};
        for (size_t i{0}; i < k; ++i) {
            number = number * 10 + stack[i];
        }

        totalJoltage += number;
    }

    std::cout << "Total joltage: " << totalJoltage << std::endl;
}
