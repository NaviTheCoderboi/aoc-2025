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

    int digits{0};

    for (u_int64_t i{n}; i > 0; i /= 10)
        digits++;
    if (digits & 1)
        return false;

    int half{digits / 2};
    uint64_t pow10{1};
    for (int i{0}; i < half; i++)
        pow10 *= 10;

    return (n % pow10) == (n / pow10);
}

/**
 * Converts a subrange to a string view.
 */
inline std::string_view asView(auto&& sub) {
    return {&*sub.begin(), static_cast<std::size_t>(std::ranges::distance(sub))};
}

int main() {
    try {
        std::uint64_t invalIdsSum{0};
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
                invalIdsSum += a;
                continue;
            }

            for (std::uint64_t id{a}; id <= b; id++) {
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