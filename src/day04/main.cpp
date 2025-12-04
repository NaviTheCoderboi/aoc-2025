#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ifstream inputFile{"input.txt"};
    if (!inputFile) {
        std::cerr << "Error opening input file.\n";
        return 1;
    }

    std::vector<std::string> paperGrid{};
    paperGrid.reserve(140); // reserve enough space for the puzzle input

    for (std::string line{}; std::getline(inputFile, line);) {
        paperGrid.emplace_back(std::move(line));
    }

    if (paperGrid.empty()) {
        std::cerr << "Input file is empty.\n";
        return 1;
    }

    const int numRows{static_cast<int>(paperGrid.size())};
    const int numCols{static_cast<int>(paperGrid[0].size())};

    if (numRows < 3 || numCols < 3) {
        std::cerr << "Grid is too small to process.\n";
        return 1;
    }

    std::vector<unsigned char> adjacentRollCount(numRows * numCols, 0);

    auto flattenIndex = [&](int row, int col) { return row * numCols + col; };

    std::deque<int> removableRollQueue{};

    static constexpr int neighborRowOffsets[8]{-1, -1, -1, 0, 0, 1, 1, 1};
    static constexpr int neighborColOffsets[8]{-1, 0, 1, -1, 1, -1, 0, 1};

    for (int row{0}; row < numRows; ++row) {
        for (int col{0}; col < numCols; ++col) {
            if (paperGrid[row][col] != '@')
                continue;

            unsigned char neighborCount{0};

            for (int dir{0}; dir < 8; ++dir) {
                const int neighborRow{row + neighborRowOffsets[dir]};
                const int neighborCol{col + neighborColOffsets[dir]};

                if (neighborRow < 0 || neighborRow >= numRows)
                    continue;
                if (neighborCol < 0 || neighborCol >= numCols)
                    continue;

                neighborCount += (paperGrid[neighborRow][neighborCol] == '@');
            }

            adjacentRollCount[flattenIndex(row, col)] = neighborCount;

            if (neighborCount < 4) {
                removableRollQueue.emplace_back(flattenIndex(row, col));
            }
        }
    }

    int totalRemovedRolls{0};

    while (!removableRollQueue.empty()) {
        const int currentIndex{removableRollQueue.front()};
        removableRollQueue.pop_front();

        const int currentRow{currentIndex / numCols};
        const int currentCol{currentIndex % numCols};

        if (paperGrid[currentRow][currentCol] != '@')
            continue;

        paperGrid[currentRow][currentCol] = '.';
        ++totalRemovedRolls;

        for (int dir{0}; dir < 8; ++dir) {
            const int neighborRow{currentRow + neighborRowOffsets[dir]};
            const int neighborCol{currentCol + neighborColOffsets[dir]};

            if (neighborRow < 0 || neighborRow >= numRows)
                continue;
            if (neighborCol < 0 || neighborCol >= numCols)
                continue;

            if (paperGrid[neighborRow][neighborCol] != '@')
                continue;

            unsigned char& neighborRollCount{
                adjacentRollCount[flattenIndex(neighborRow, neighborCol)]};

            if (--neighborRollCount < 4) {
                removableRollQueue.emplace_back(flattenIndex(neighborRow, neighborCol));
            }
        }
    }

    std::cout << "Total removed paper rolls: " << totalRemovedRolls << '\n';

    return 0;
}
