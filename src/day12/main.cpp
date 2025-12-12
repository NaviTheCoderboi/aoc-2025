#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Shape {
    std::vector<std::string> rows{};
    std::uint32_t area{};
};

int main() {
    std::ifstream file("input.txt");
    if (!file.good()) {
        std::cout << "0\n";
        return 0;
    }

    std::vector<Shape> shapes{};
    std::string line{};

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        if (line.back() != ':')
            break;

        Shape shape{};

        while (std::getline(file, line) && !line.empty()) {
            shape.area += static_cast<std::uint32_t>(std::count(line.begin(), line.end(), '#'));
            shape.rows.push_back(line);
        }

        shapes.push_back(shape);
    }

    std::uint64_t fitCount{};

    do {
        if (line.empty())
            continue;

        std::size_t xPos{line.find('x')};
        std::size_t colonPos{line.find(':', xPos)};
        if (xPos == std::string::npos || colonPos == std::string::npos)
            continue;

        std::uint32_t width{static_cast<std::uint32_t>(std::stoul(line.substr(0, xPos)))};
        std::uint32_t height{static_cast<std::uint32_t>(std::stoul(line.substr(xPos + 1, colonPos - xPos - 1)))};

        std::vector<std::uint64_t> counts{};
        counts.reserve(shapes.size());

        std::string_view tail{line};
        tail.remove_prefix(colonPos + 1);

        std::size_t i{}, start{};
        while (i < tail.size()) {
            while (i < tail.size() && tail[i] == ' ')
                i++;

            start = i;
            while (i < tail.size() && tail[i] != ' ')
                i++;

            if (i > start)
                counts.push_back(std::stoull(std::string{tail.substr(start, i - start)}));
        }

        std::uint64_t totalArea{};
        for (std::size_t idx{}; idx < counts.size(); idx++)
            totalArea += counts[idx] * shapes[idx].area;

        if (totalArea <= static_cast<std::uint64_t>(width) * height)
            fitCount++;

    } while (std::getline(file, line));

    std::cout << fitCount << "\n";
    return 0;
}
