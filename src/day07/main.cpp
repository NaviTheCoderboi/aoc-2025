#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ifstream inputFile{"input.txt"};
    if (!inputFile) {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    std::vector<std::string> grid{};
    grid.reserve(142); // pre-allocated for input.txt but will work for other sizes too

    std::string line{};
    while (std::getline(inputFile, line)) {
        grid.push_back(line);
    }

    if (grid.empty()) {
        std::cout << "No data found in file.\n";
        return 0;
    }

    const int totalRows{static_cast<int>(grid.size())};
    const int totalCols{static_cast<int>(grid[0].size())};

    int startCol{0};
    for (int col{0}; col < totalCols; ++col) {
        if (grid[0][col] == 'S') {
            startCol = col;
            break;
        }
    }

    std::vector<std::uint64_t> currentRowTimelines(totalCols, 0);
    std::vector<std::uint64_t> nextRowTimelines(totalCols, 0);

    currentRowTimelines[startCol] = 1;

    std::uint64_t finishedTimelineCount{0};

    for (int row{0}; row < totalRows - 1; ++row) {
        std::fill(nextRowTimelines.begin(), nextRowTimelines.end(), 0);

        const std::string& belowRow{grid[row + 1]};

        for (int col{0}; col < totalCols; ++col) {

            const std::uint64_t timelinesHere{currentRowTimelines[col]};
            if (timelinesHere == 0)
                continue;

            const char cellBelow{belowRow[col]};

            if (cellBelow == '.') {
                nextRowTimelines[col] += timelinesHere;
            } else if (cellBelow == '^') {

                if (col > 0)
                    nextRowTimelines[col - 1] += timelinesHere;

                if (col + 1 < totalCols)
                    nextRowTimelines[col + 1] += timelinesHere;
            }
        }

        currentRowTimelines.swap(nextRowTimelines);
    }

    for (int col{0}; col < totalCols; ++col) {
        finishedTimelineCount += currentRowTimelines[col];
    }

    std::cout << "Total finished timelines: " << finishedTimelineCount << '\n';

    return 0;
}
