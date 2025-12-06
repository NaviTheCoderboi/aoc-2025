#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using i128 = __int128_t;

std::ostream& operator<<(std::ostream& output, i128 value) {
    if (value == 0)
        return output << '0';

    if (value < 0) {
        output << '-';
        value = -value;
    }

    std::string digits{};
    while (value > 0) {
        digits.push_back(char('0' + (value % 10)));
        value /= 10;
    }

    for (auto it = digits.rbegin(); it != digits.rend(); ++it)
        output << *it;

    return output;
}

int main() {
    std::ifstream inputFile{"input.txt"};
    if (!inputFile.is_open()) {
        std::cout << "Failed to open input file\n";
        return 1;
    }

    std::vector<std::string> grid{};
    std::string line{};

    while (std::getline(inputFile, line))
        grid.push_back(line);

    if (grid.empty())
        return 0;

    const int gridHeight{static_cast<int>(grid.size())};
    const int gridWidth{static_cast<int>(grid[0].size())};

    std::vector<bool> isSeparatorColumn(gridWidth, true);

    for (int col{0}; col < gridWidth; ++col) {
        for (int row{0}; row < gridHeight; ++row) {
            if (col < static_cast<int>(grid[row].size()) && grid[row][col] != ' ') {
                isSeparatorColumn[col] = false;
                break;
            }
        }
    }

    std::vector<int> problemStartColumns{};
    std::vector<int> problemEndColumns{};

    for (int col{0}; col < gridWidth;) {
        while (col < gridWidth && isSeparatorColumn[col])
            ++col;

        if (col >= gridWidth)
            break;

        int startColumn{col};

        while (col < gridWidth && !isSeparatorColumn[col])
            ++col;

        int endColumn{col - 1};

        problemStartColumns.push_back(startColumn);
        problemEndColumns.push_back(endColumn);
    }

    i128 finalAnswer{0};

    for (std::size_t problemIndex{0}; problemIndex < problemStartColumns.size(); ++problemIndex) {
        const int startColumn{problemStartColumns[problemIndex]};
        const int endColumn{problemEndColumns[problemIndex]};

        char operation{'?'};

        for (int col{startColumn}; col <= endColumn; ++col) {
            if (col < static_cast<int>(grid[gridHeight - 1].size()) &&
                grid[gridHeight - 1][col] != ' ') {

                operation = grid[gridHeight - 1][col];
                break;
            }
        }

        i128 problemResult{(operation == '*') ? 1 : 0};

        for (int col{endColumn}; col >= startColumn; --col) {

            i128 columnValue{0};
            bool hasDigit{false};

            for (int row{0}; row < gridHeight - 1; ++row) {
                if (col < static_cast<int>(grid[row].size()) &&
                    std::isdigit(static_cast<unsigned char>(grid[row][col]))) {

                    columnValue = columnValue * 10 + (grid[row][col] - '0');
                    hasDigit = true;
                }
            }

            if (!hasDigit)
                continue;

            if (operation == '*')
                problemResult *= columnValue;
            else
                problemResult += columnValue;
        }

        finalAnswer += problemResult;
    }

    std::cout << "Final Answer: " << finalAnswer << "\n";
    return 0;
}
