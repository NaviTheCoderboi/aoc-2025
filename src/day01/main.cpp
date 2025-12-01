#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

/**
 * The direction in which dial rotation occurs.
 */
enum class Direction {
    LEFT,
    RIGHT,
};

/**
 * Represents a rotation of a dial.
 */
struct Rotation {
    /** The number of ticks to rotate */
    int ticks;
    /** The direction of rotation */
    Direction direction;
};

/**
 * Reads raw rotation instructions from a file.
 *
 * @param filename The name of the file containing rotation instructions.
 * @return A vector of strings, each representing a raw rotation instruction.
 * @throws std::runtime_error if the file cannot be opened.
 */
std::vector<std::string> getRawRotations(const std::string& filename) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Could not open file: " + filename);

    std::vector<std::string> lines{};

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(std::move(line));
    }

    return lines;
}

/**
 * Parses raw rotation instructions into Rotation objects.
 *
 * @param rawRotations A vector of strings representing raw rotation instructions.
 * @return A vector of Rotation objects.
 * @throws std::runtime_error if any instruction is invalid.
 */
std::vector<Rotation> parseRotations(const std::vector<std::string>& rawRotations) {
    std::vector<Rotation> rotations;
    const std::regex rotationPattern(R"(^([LR])(\d+)$)");

    for (const auto& raw : rawRotations) {
        if (raw.size() < 2)
            throw std::runtime_error("Invalid rotation format: " + raw);

        std::smatch match;
        if (!std::regex_match(raw, match, rotationPattern))
            throw std::runtime_error("Invalid rotation format: " + raw);

        rotations.emplace_back(
            Rotation{.ticks = std::stoi(match[2]),
                     .direction = (match[1] == "L") ? Direction::LEFT : Direction::RIGHT});
    }

    return rotations;
}

/**
 * Rotates the current position based on the given rotation.
 *
 * @param currentPositon The current position of the dial (0-99).
 * @param rotation The rotation to apply.
 */
void rotate(int& pos, const Rotation& rot, int& zeroCount) {
    int steps{rot.ticks};
    if (steps == 0)
        return;

    int newPos{0};

    if (rot.direction == Direction::RIGHT) {
        newPos = (pos + steps) % 100;

        if (pos == 0) {
            zeroCount += steps / 100;
        } else {
            int firstCrossing = 100 - pos;
            if (steps >= firstCrossing) {
                zeroCount += 1 + (steps - firstCrossing) / 100;
            }
        }

    } else {
        newPos = ((pos - steps) % 100 + 100) % 100;

        if (pos == 0) {
            zeroCount += steps / 100;
        } else {
            if (steps >= pos) {
                zeroCount += 1 + (steps - pos) / 100;
            }
        }
    }

    pos = newPos;
}

int main() {
    try {
        const auto rawRotations{getRawRotations("input.txt")};
        const auto rotations{parseRotations(rawRotations)};

        int zeroCount{0};

        int currentPosition{50};
        for (const auto& rotation : rotations) {
            rotate(currentPosition, rotation, zeroCount);
        }

        std::cout << "Password: " << zeroCount << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}