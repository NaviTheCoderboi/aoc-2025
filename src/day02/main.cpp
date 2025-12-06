#include <charconv>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

bool isInvalid(uint64_t n) {
    if (n == 0)
        return true;

    char buf[20];
    char* p{buf + 20};

    uint64_t x{n};
    while (x > 0) {
        *--p = '0' + (x % 10);
        x /= 10;
    }

    std::string_view sv{p, static_cast<std::size_t>(buf + 20 - p)};
    const std::size_t len{sv.size()};

    for (std::size_t psize{1}; psize <= len / 2; ++psize) {
        if (len % psize != 0)
            continue;

        bool ok{true};
        for (std::size_t i{psize}; i < len; ++i) {
            if (sv[i] != sv[i % psize]) {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;
    }
    return false;
}

int main() {
    std::uint64_t invalIdsSum{0};

    std::ifstream file{"input.txt"};
    if (!file) {
        std::cerr << "Failed to open input.txt\n";
        return 1;
    }

    std::string line{};
    std::getline(file, line);

    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    const char* s{line.data()};
    const char* end{s + line.size()};

    while (s < end) {
        const char* comma{reinterpret_cast<const char*>(std::memchr(s, ',', end - s))};
        if (!comma)
            comma = end;

        const char* dash{reinterpret_cast<const char*>(std::memchr(s, '-', comma - s))};
        if (!dash) {
            std::cerr << "Invalid range (missing '-')\n";
            return 1;
        }

        std::uint64_t a{0}, b{0};

        auto r1{std::from_chars(s, dash, a)};
        if (r1.ec != std::errc{})
            return 1;

        auto r2{std::from_chars(dash + 1, comma, b)};
        if (r2.ec != std::errc{})
            return 1;

        if (a == b) {
            if (isInvalid(a))
                invalIdsSum += a;
        } else {
            for (std::uint64_t id{a}; id <= b; ++id)
                if (isInvalid(id))
                    invalIdsSum += id;
        }

        s = comma + (comma != end);
    }

    std::cout << "Sum of invalid IDs: " << invalIdsSum << "\n";
}
