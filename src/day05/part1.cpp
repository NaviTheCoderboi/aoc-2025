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

    std::vector<std::string> freshIngeredientsRanges{};
    std::vector<std::string> ingredientIds{};
    bool readingRanges{true};
    for (std::string line; std::getline(file, line);) {
        if (line.empty()) {
            readingRanges = false;
            continue;
        }
        if (readingRanges) {
            freshIngeredientsRanges.push_back(line);
        } else {
            ingredientIds.push_back(line);
        }
    }

    int freshIngredientsCount{};
    for (const auto& ingredientId : ingredientIds) {
        for (const auto& range : freshIngeredientsRanges) {
            size_t dashPos = range.find('-');
            if (dashPos == std::string::npos) {
                continue;
            }

            std::uint64_t rangeStart{std::stoull(range.substr(0, dashPos))};
            std::uint64_t rangeEnd{std::stoull(range.substr(dashPos + 1))};

            std::uint64_t ingredientNumber{std::stoull(ingredientId)};
            if (ingredientNumber >= rangeStart && ingredientNumber <= rangeEnd) {
                ++freshIngredientsCount;
                break;
            }
        }
    }

    std::cout << "\nTotal Fresh Ingredients: " << freshIngredientsCount << '\n';

    return 0;
}