#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

struct Edge {
    std::int64_t distanceSquared;
    int boxA;
    int boxB;
};

int findCircuitRoot(std::vector<int>& parent, int box) {
    int root{box};

    while (parent[root] >= 0)
        root = parent[root];

    while (box != root) {
        int next{parent[box]};
        parent[box] = root;
        box = next;
    }
    return root;
}

bool mergeCircuits(std::vector<int>& parent, int boxA, int boxB) {
    boxA = findCircuitRoot(parent, boxA);
    boxB = findCircuitRoot(parent, boxB);

    if (boxA == boxB)
        return false;

    if (-parent[boxA] < -parent[boxB])
        std::swap(boxA, boxB);

    parent[boxA] += parent[boxB];
    parent[boxB] = boxA;
    return true;
}

int main() {
    std::ifstream file("input.txt");
    if (!file)
        return 1;

    std::vector<std::array<int, 3>> junctionBoxes;
    junctionBoxes.reserve(1000);
    int x, y, z;
    while (file >> x) {
        file.ignore(1);
        file >> y;
        file.ignore(1);
        file >> z;
        junctionBoxes.push_back({x, y, z});
    }

    int numBoxes{static_cast<int>(junctionBoxes.size())};
    std::vector<Edge> edges;
    edges.reserve(numBoxes * (numBoxes - 1) / 2);

    for (int i{0}; i < numBoxes; i++) {
        auto& boxI{junctionBoxes[i]};

        for (int j{i + 1}; j < numBoxes; j++) {
            auto& boxJ{junctionBoxes[j]};

            std::int64_t dx{static_cast<std::int64_t>(boxI[0]) - boxJ[0]};
            std::int64_t dy{static_cast<std::int64_t>(boxI[1]) - boxJ[1]};
            std::int64_t dz{static_cast<std::int64_t>(boxI[2]) - boxJ[2]};

            edges.push_back({dx * dx + dy * dy + dz * dz, i, j});
        }
    }

    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) { return a.distanceSquared < b.distanceSquared; });

    std::vector<int> parent(numBoxes, -1);
    int numCircuits{numBoxes};
    int lastBoxA{-1}, lastBoxB{-1};

    for (const auto& e : edges) {
        if (mergeCircuits(parent, e.boxA, e.boxB)) {
            lastBoxA = e.boxA;
            lastBoxB = e.boxB;
            numCircuits--;
            if (numCircuits == 1)
                break;
        }
    }

    std::int64_t result{static_cast<std::int64_t>(junctionBoxes[lastBoxA][0]) *
                        static_cast<std::int64_t>(junctionBoxes[lastBoxB][0])};

    std::cout << result << '\n';
}
