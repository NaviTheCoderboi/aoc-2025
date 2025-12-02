#include <fstream>
#include <iostream>

// faster version
int main() {
    try {
        int zeroCount{0};
        int currentPosition{50};

        std::ifstream file("input.txt");

        for (std::string line; std::getline(file, line);) {
            char direction = line[0];
            int ticks = std::stoi(line.substr(1));

            if (direction == 'R') {
                int newPos = (currentPosition + ticks) % 100;

                if (currentPosition == 0) {
                    zeroCount += ticks / 100;
                } else {
                    int firstCrossing = 100 - currentPosition;
                    if (ticks >= firstCrossing) {
                        zeroCount += 1 + (ticks - firstCrossing) / 100;
                    }
                }

                currentPosition = newPos;
            } else if (direction == 'L') {
                int newPos = ((currentPosition - ticks) % 100 + 100) % 100;

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

        std::cout << "Password: " << zeroCount << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}