#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::ifstream file{"input.txt"};
    if (!file) {
        std::cerr << "Failed to open input file\n";
        return 1;
    }

    std::string line{};
    std::uint64_t total{0};

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        std::stringstream ss{line};
        std::string token{};

        ss >> token;
        std::bitset<256> target{};
        int lightCount{0};

        for (char c : token) {
            if (c == '.' || c == '#')
                target[lightCount++] = (c == '#');
        }

        std::vector<std::bitset<256>> buttons{};

        while (ss >> token) {
            if (token[0] == '{')
                break;

            std::bitset<256> mask{};
            std::string inside{token.substr(1, token.size() - 2)};
            std::stringstream ss2{inside};
            std::string indexStr{};

            while (std::getline(ss2, indexStr, ',')) {
                if (!indexStr.empty())
                    mask[std::stoi(indexStr)] = 1;
            }
            buttons.push_back(mask);
        }

        int nButtons{static_cast<int>(buttons.size())};
        int best{1000000000};

        for (int m{0}; m < (1 << nButtons); m++) {
            std::bitset<256> state{};
            int presses{0};

            for (int i{0}; i < nButtons; i++) {
                if (m & (1 << i)) {
                    state ^= buttons[i];
                    presses++;
                }
            }

            if (state == target)
                best = std::min(best, presses);
        }

        total += best;
    }

    std::cout << total << "\n";
    return 0;
}
