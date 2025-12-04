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

    std::vector<std::string> grid;
    for (std::string line; std::getline(inputFile, line);) {
        grid.push_back(line);
    }
    inputFile.close();

    if (grid.empty()) {
        std::cerr << "Input file is empty." << std::endl;
        return 1;
    }

    int totalRows{static_cast<int>(grid.size())};
    int totalColumns{grid.empty() ? 0 : static_cast<int>(grid[0].size())};

    int accessiblePaperRoles{0};

    for (int currentRow{0}; currentRow < totalRows; ++currentRow) {
        for (int currentColumn{0}; currentColumn < totalColumns; ++currentColumn) {
            char currentChar{grid[currentRow][currentColumn]};
            if (currentChar != '@')
                continue;

            int neighborCount{0};
            for (int rowOffset{-1}; rowOffset <= 1; ++rowOffset) {
                for (int columnOffset{-1}; columnOffset <= 1; ++columnOffset) {
                    if (rowOffset == 0 && columnOffset == 0)
                        continue;

                    int neighborRow{static_cast<int>(currentRow) + rowOffset};
                    int neighborColumn{static_cast<int>(currentColumn) + columnOffset};

                    if (neighborRow < 0 || neighborRow >= totalRows) {
                        continue;
                    }
                    if (neighborColumn < 0 || neighborColumn >= totalColumns) {
                        continue;
                    }

                    if (grid[neighborRow][neighborColumn] == '@') {
                        ++neighborCount;
                    }
                }
            }

            if (neighborCount < 4) {
                ++accessiblePaperRoles;
            }
        }
    }

    std::cout << "Accessible paper roles: " << accessiblePaperRoles << std::endl;

    return 0;
}