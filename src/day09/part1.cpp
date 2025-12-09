#include <algorithm>
#include <cstdint>
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

    std::vector<std::pair<int, int>> points{};
    points.reserve(496); // Reserve space for 496 points based on input.txt but will work for any size

    std::string line;
    while (std::getline(file, line)) {
        size_t commaPos{line.find(',')};
        if (commaPos != std::string::npos) {
            int x{std::stoi(line.substr(0, commaPos))};
            int y{std::stoi(line.substr(commaPos + 1))};

            points.emplace_back(x, y);
        }
    }

    std::vector<std::uint64_t> areas{};
    for (int a{0}; a < points.size(); a++) {
        auto [x1, y1]{points[a]};

        for (int b{1}; b < points.size(); b++) {
            auto [x2, y2]{points[b]};

            std::uint64_t area{(static_cast<std::uint64_t>(std::abs(x2 - x1)) + 1) *
                               (static_cast<std::uint64_t>(std::abs(y2 - y1)) + 1)};
            areas.push_back(area);
        }
    }

    std::sort(areas.begin(), areas.end());
    std::cout << "Largest area: " << areas.back() << '\n';

    return 0;
}