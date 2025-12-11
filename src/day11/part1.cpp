#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// represents the node in the device graph
using Node = std::string;

std::unordered_map<Node, std::uint64_t> cache{};

std::uint64_t dfs(const Node& node, std::unordered_map<Node, std::vector<std::string>>& graph) {
    if (node == "out")
        return 1;

    if (cache.count(node)) {
        return cache[node];
    }

    std::uint64_t ways{0};
    for (const std::string& nxt : graph[node]) {
        ways += dfs(nxt, graph);
    }

    cache[node] = ways;
    return ways;
};

int main() {
    std::ifstream file("input.txt");
    if (!file || !file.good()) {
        std::cerr << "Failed to open input file\n";
        return 1;
    }

    std::unordered_map<Node, std::vector<std::string>> graph{};
    graph.reserve(574);

    std::string line;
    while (getline(file, line)) {
        if (line.empty())
            continue;

        auto pos{static_cast<int>(line.find(':'))};
        std::string name{line.substr(0, pos)};

        std::vector<std::string> children;
        int i{pos + 1};

        while (i < line.size() && line[i] == ' ')
            i++;

        while (i < line.size()) {
            int start{i};
            while (i < line.size() && line[i] != ' ')
                i++;

            children.push_back(line.substr(start, i - start));

            while (i < line.size() && line[i] == ' ')
                i++;
        }

        graph[name] = std::move(children);
    }

    auto result{dfs("you", graph)};

    std::cout << result << "\n";
    return 0;
}