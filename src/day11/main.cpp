#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct KeyHash {
    std::size_t operator()(const std::uint32_t k) const noexcept {
        return std::hash<std::uint32_t>{}(k);
    }
};

int main() {
    std::ifstream file{"input.txt"};
    if (!file.is_open()) {
        std::cerr << "Failed to open input file\n";
        return 1;
    }

    std::unordered_map<std::string, int> nameToId{};
    nameToId.reserve(574);

    auto getId{[&](const std::string& name) -> int {
        auto it{nameToId.find(name)};
        if (it != nameToId.end())
            return it->second;

        int id{static_cast<int>(nameToId.size())};
        nameToId.emplace(name, id);

        return id;
    }};

    std::vector<std::vector<int>> graph{};
    graph.reserve(574);

    std::string line{};
    while (getline(file, line)) {
        if (line.empty())
            continue;

        std::size_t pos{line.find(':')};
        if (pos == std::string::npos)
            continue;

        std::string parent{line.substr(0, pos)};
        int parentId{getId(parent)};

        if (parentId >= static_cast<int>(graph.size()))
            graph.resize(parentId + 1);

        std::size_t i{pos + 1};
        while (i < line.size() && line[i] == ' ')
            ++i;

        while (i < line.size()) {
            std::size_t start{i};
            while (i < line.size() && line[i] != ' ')
                ++i;

            std::string child{line.substr(start, i - start)};
            int childId{getId(child)};

            if (childId >= static_cast<int>(graph.size()))
                graph.resize(childId + 1);

            graph[parentId].push_back(childId);

            while (i < line.size() && line[i] == ' ')
                ++i;
        }
    }

    int svrId{getId("svr")};
    int outId{getId("out")};
    int dacId{getId("dac")};
    int fftId{getId("fft")};

    std::vector<std::uint64_t> memo{};
    memo.resize(nameToId.size() * 4, UINT64_MAX);

    std::function<std::uint64_t(int, bool, bool)> dfs = [&](int nodeId, bool seenDac, bool seenFft) -> std::uint64_t {
        if (nodeId == outId)
            return (seenDac && seenFft) ? 1 : 0;

        std::uint32_t key{static_cast<std::uint32_t>(nodeId * 4 + (seenDac << 1) + seenFft)};
        if (memo[key] != UINT64_MAX)
            return memo[key];

        bool nextDac{seenDac || (nodeId == dacId)};
        bool nextFft{seenFft || (nodeId == fftId)};

        std::uint64_t ways{0};
        for (int childId : graph[nodeId])
            ways += dfs(childId, nextDac, nextFft);

        memo[key] = ways;
        return ways;
    };

    std::uint64_t result{dfs(svrId, false, false)};
    std::cout << result << "\n";

    return 0;
}
