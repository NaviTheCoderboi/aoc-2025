#include <algorithm>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using Range = std::pair<std::uint64_t, std::uint64_t>;

inline bool parseUnsigned(std::string_view text, std::uint64_t& value) {
    const auto result{std::from_chars(text.data(), text.data() + text.size(), value)};
    return result.ec == std::errc{} && result.ptr == text.data() + text.size();
}

int main() {
    std::ifstream inputFile{"input.txt"};
    if (!inputFile.is_open()) {
        std::cerr << "Error: Failed to open input.txt\n";
        return 1;
    }

    std::vector<Range> ingredientRanges{};
    ingredientRanges.reserve(187); // preallocate based on known input size but will work generally

    for (std::string line{}; std::getline(inputFile, line);) {
        if (line.empty()) {
            break;
        }

        const std::size_t dashPos{line.find('-')};
        if (dashPos == std::string::npos) {
            std::cerr << "Error: Invalid range format: " << line << "\n";
            return 1;
        }

        const std::string_view startText{line.data(), dashPos};
        const std::string_view endText{line.data() + dashPos + 1, line.size() - dashPos - 1};

        std::uint64_t rangeStart{};
        std::uint64_t rangeEnd{};

        if (!parseUnsigned(startText, rangeStart) || !parseUnsigned(endText, rangeEnd)) {
            std::cerr << "Error: Invalid numeric values in range: " << line << "\n";
            return 1;
        }

        if (rangeStart > rangeEnd) {
            std::cerr << "Error: Range start > end: " << line << "\n";
            return 1;
        }

        ingredientRanges.emplace_back(rangeStart, rangeEnd);
    }

    if (ingredientRanges.empty()) {
        std::cout << "Total fresh ingredients: 0\n";
        return 0;
    }

    std::sort(ingredientRanges.begin(), ingredientRanges.end());

    std::vector<Range> mergedRanges{};
    mergedRanges.reserve(ingredientRanges.size());

    std::uint64_t currentStart{ingredientRanges[0].first};
    std::uint64_t currentEnd{ingredientRanges[0].second};

    for (std::size_t i{1}; i < ingredientRanges.size(); ++i) {
        const auto& [nextStart, nextEnd] = ingredientRanges[i];

        if (nextStart <= currentEnd || nextStart - currentEnd == 1) {
            currentEnd = std::max(currentEnd, nextEnd);
        } else {
            mergedRanges.emplace_back(currentStart, currentEnd);
            currentStart = nextStart;
            currentEnd = nextEnd;
        }
    }

    mergedRanges.emplace_back(currentStart, currentEnd);

    std::uint64_t totalFreshIngredientCount{0};

    for (const auto& [start, end] : mergedRanges) {
        totalFreshIngredientCount += (end - start + 1);
    }

    std::cout << "Total fresh ingredients: " << totalFreshIngredientCount << "\n";
    return 0;
}