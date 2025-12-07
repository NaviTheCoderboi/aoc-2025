#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    if (lines.empty()) {
        std::cout << "No data found in the file." << std::endl;
        return 0;
    }

    int rows{static_cast<int>(lines.size())};
    int cols{static_cast<int>(lines[0].size())};

    int startRow{0}, startCol{0};
    for (int c{0}; c < cols; ++c) {
        if (lines[0][c] == 'S') {
            startCol = c;
            break;
        }
    }

    std::set<std::pair<int, int>> activeBeams;
    activeBeams.insert({startRow, startCol});

    int splitCount{0};

    while (!activeBeams.empty()) {
        std::set<std::pair<int, int>> newBeams;

        for (const auto& beam : activeBeams) {
            int newRow{beam.first + 1};

            if (newRow >= rows)
                continue;

            char cell{lines[newRow][beam.second]};

            if (cell == '.') {
                newBeams.insert({newRow, beam.second});
            } else if (cell == '^') {
                splitCount++;

                int leftCol{beam.second - 1};
                int rightCol{beam.second + 1};

                if (leftCol >= 0)
                    newBeams.insert({newRow, leftCol});

                if (rightCol < cols)
                    newBeams.insert({newRow, rightCol});
            }
        }

        activeBeams = std::move(newBeams);
    }

    std::cout << "Total splits: " << splitCount << std::endl;

    return 0;
}