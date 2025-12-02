#include <fstream>
#include <iostream>
#include <stdexcept>

// faster version
int main() {
    try {
        int zeroCount{0};
        int currentPosition{50};

        std::ifstream file("input.txt");
        if (!file)
            throw std::runtime_error("Failed to open input file.");

        for (std::string line; std::getline(file, line);) {
            const char direction{line[0]};
            const int ticks{std::stoi(line.substr(1))};

            if (direction == 'R') {
                const int newPos{(currentPosition + ticks) % 100};

                if (currentPosition == 0) {
                    zeroCount += ticks / 100;
                } else {
                    const int firstCrossing = 100 - currentPosition;
                    if (ticks >= firstCrossing) {
                        zeroCount += 1 + (ticks - firstCrossing) / 100;
                    }
                }

                currentPosition = newPos;
            } else if (direction == 'L') {
                const int newPos{((currentPosition - ticks) % 100 + 100) % 100};
                if (currentPosition == 0) {
                    zeroCount += ticks / 100;
                } else {
                    if (ticks >= currentPosition) {
                        zeroCount += 1 + (ticks - currentPosition) / 100;
                    }
                }

                currentPosition = newPos;
            }
        }

        std::cout << "Password: " << zeroCount << "\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}