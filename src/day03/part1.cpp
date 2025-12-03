#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int joltage{0};

    for (std::string line; std::getline(inputFile, line);) {
        int right{0};
        int pair{0};

        for (char c : line) {
            int num{c - '0'};

            if (right != 0) {
                int newPair{right * 10 + num};
                if (newPair > pair) {
                    pair = newPair;
                }
            }

            if (num > right) {
                right = num;
            }
        }

        joltage += pair;
    }

    std::cout << "Total joltage: " << joltage << std::endl;

    return 0;
}