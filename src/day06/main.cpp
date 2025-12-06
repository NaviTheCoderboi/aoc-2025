#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ifstream inputFile{"input.txt"};
    if (!inputFile) {
        std::cout << "Failed to open input file\n";
        return 1;
    }

    std::vector<std::string> grid{};
    for (std::string line{}; std::getline(inputFile, line);)
        grid.push_back(line);

    if (grid.empty())
        return 0;

    const int gridHeight{static_cast<int>(grid.size())};
    const int gridWidth{static_cast<int>(grid[0].size())};

    std::vector<bool> isSeparatorColumn(gridWidth, true);

    for (int col{}; col < gridWidth; ++col) {
        for (int row{}; row < gridHeight; ++row) {
            if (col < static_cast<int>(grid[row].size()) && grid[row][col] != ' ') {
                isSeparatorColumn[col] = false;
                break;
            }
        }
    }

    std::vector<int> problemStartColumns{};
    std::vector<int> problemEndColumns{};

    for (int col{}; col < gridWidth;) {
        while (col < gridWidth && isSeparatorColumn[col])
            ++col;

        if (col >= gridWidth)
            break;

        const int startColumn{col};

        while (col < gridWidth && !isSeparatorColumn[col])
            ++col;

        const int endColumn{col - 1};

        problemStartColumns.push_back(startColumn);
        problemEndColumns.push_back(endColumn);
    }

    std::uint64_t finalAnswer{0};

    for (std::size_t problemIndex{}; problemIndex < problemStartColumns.size(); ++problemIndex) {
        const int startColumn{problemStartColumns[problemIndex]};
        const int endColumn{problemEndColumns[problemIndex]};

        char operation{'+'};
        const std::string& opRow{grid[gridHeight - 1]};

        for (int col{startColumn}; col <= endColumn && col < static_cast<int>(opRow.size());
             ++col) {
            if (opRow[col] != ' ') {
                operation = opRow[col];
                break;
            }
        }

        std::uint64_t problemResult{operation == '*' ? 1ULL : 0ULL};

        for (int col{endColumn}; col >= startColumn; --col) {
            std::uint64_t columnValue{0};
            bool hasDigit{false};

            for (int row{}; row < gridHeight - 1; ++row) {
                if (col < static_cast<int>(grid[row].size())) {
                    const unsigned char ch{static_cast<unsigned char>(grid[row][col])};
                    if (std::isdigit(ch)) {
                        columnValue = columnValue * 10 + (ch - '0');
                        hasDigit = true;
                    }
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
