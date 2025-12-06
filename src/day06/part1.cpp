#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ifstream file("input.txt");
    if (!file || !file.good()) {
        std::cerr << "Failed to open input file\n";
        return 1;
    }

    std::vector<std::vector<std::uint64_t>> grid{};
    std::vector<char> functions{}; // + or *

    std::string line;
    bool readingNumbers{true};

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        bool hasOperator{false};
        for (char c : line) {
            if (c == '+' || c == '*') {
                hasOperator = true;
                break;
            }
        }

        if (hasOperator) {
            for (char c : line) {
                if (c == '+' || c == '*') {
                    functions.push_back(c);
                }
            }
            readingNumbers = false;
        } else if (readingNumbers) {
            std::vector<std::uint64_t> row{};
            std::string numStr;

            for (std::size_t i{0}; i <= line.size(); ++i) {
                if (i == line.size() || line[i] == ' ') {
                    if (!numStr.empty()) {
                        row.push_back(std::stoull(numStr));
                        numStr.clear();
                    }
                } else {
                    numStr += line[i];
                }
            }
            if (!row.empty()) {
                grid.push_back(row);
            }
        }
    }

    std::uint64_t result{0};

    int cols{static_cast<int>(grid[0].size())};
    int rows{static_cast<int>(grid.size())};

    for (int col{0}; col < cols; ++col) {
        std::uint64_t colResult{grid[0][col]};

        for (int row{1}; row < rows; ++row) {
            if (functions[col] == '+') {
                colResult += grid[row][col];
            } else if (functions[col] == '*') {
                colResult *= grid[row][col];
            }
        }

        result += colResult;
    }

    std::cout << "Result: " << result << "\n";
    return 0;
}
