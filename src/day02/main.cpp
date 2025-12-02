#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>

/**
 * Converts a string view to an unsigned integer.
 */
uint64_t toUint(std::string_view sv) {
    uint64_t v{};

    auto res{std::from_chars(sv.data(), sv.data() + sv.size(), v)};
    if (res.ec != std::errc())
        throw std::runtime_error("parse error");

    return v;
}

bool isInvalid(uint64_t n) {
    if (n == 0)
        return true;

    char buffer[20];
    auto [ptr, ec]{std::to_chars(buffer, buffer + 20, n)};
    if (ec != std::errc())
        return false;

    const int len{static_cast<int>(ptr - buffer)};

    for (int pattern_len{1}; pattern_len <= len / 2; ++pattern_len) {
        if (len % pattern_len != 0)
            continue;

        bool matches{true};
        for (int i{pattern_len}; i < len; ++i) {
            if (buffer[i] != buffer[i % pattern_len]) {
                matches = false;
                break;
            }
        }

        if (matches)
            return true;
    }

    return false;
}

/**
 * Converts a subrange to a string view.
 */
inline std::string_view asView(auto&& sub) {
    return {&*sub.begin(), static_cast<size_t>(std::ranges::distance(sub))};
}

int main() {
    try {
        uint64_t invalIdsSum{0};
        std::ifstream file("input.txt");
        if (!file)
            throw std::runtime_error("Could not open the file.");

        std::string line;
        std::getline(file, line);
        file.close();

        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        for (const auto part : std::views::split(line, ',')) {
            auto range{asView(part)};

            auto pieces{std::views::split(range, '-')};
            auto it{pieces.begin()};
            if (it == pieces.end())
                throw std::runtime_error("Invalid range format.");

            auto a{toUint(asView(*it++))};
            if (it == pieces.end())
                throw std::runtime_error("Invalid range format.");

            auto b{toUint(asView(*it))};

            if (a == b) {
                if (isInvalid(a))
                    invalIdsSum += a;
                continue;
            }

            for (uint64_t id{a}; id <= b; ++id) {
                if (isInvalid(id))
                    invalIdsSum += id;
            }
        }

        std::cout << "Sum of invalid IDs: " << invalIdsSum << "\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}