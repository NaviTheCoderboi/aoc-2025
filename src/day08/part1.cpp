#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

int findCircuitRoot(std::vector<int>& circuitParent, int box) {
    int root{box};
    while (circuitParent[root] >= 0)
        root = circuitParent[root];

    while (box != root) {
        int next{circuitParent[box]};
        circuitParent[box] = root;
        box = next;
    }

    return root;
}

void mergeCircuits(std::vector<int>& circuitParent, int boxA, int boxB) {
    boxA = findCircuitRoot(circuitParent, boxA);
    boxB = findCircuitRoot(circuitParent, boxB);

    if (boxA == boxB)
        return;

    if (-circuitParent[boxA] < -circuitParent[boxB])
        std::swap(boxA, boxB);

    circuitParent[boxA] += circuitParent[boxB];
    circuitParent[boxB] = boxA;
}

int main() {
    std::ifstream file("input.txt");
    if (!file || !file.good()) {
        std::cerr << "Failed to open input file\n";
        return 1;
    }

    std::vector<std::array<int, 3>> junctionBoxes{};
    std::string line;
    while (std::getline(file, line)) {
        int x, y, z;
        sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
        junctionBoxes.push_back({x, y, z});
    }

    int numBoxes{static_cast<int>(junctionBoxes.size())};
    constexpr int numConnections{1000};

    using Edge = std::tuple<std::int64_t, int, int>;
    std::priority_queue<Edge> maxHeap;
    for (int i{0}; i < numBoxes; i++) {
        for (int j{i + 1}; j < numBoxes; j++) {
            std::int64_t dx{static_cast<std::int64_t>(junctionBoxes[i][0]) - junctionBoxes[j][0]};
            std::int64_t dy{static_cast<std::int64_t>(junctionBoxes[i][1]) - junctionBoxes[j][1]};
            std::int64_t dz{static_cast<std::int64_t>(junctionBoxes[i][2]) - junctionBoxes[j][2]};

            std::int64_t distanceSquared{dx * dx + dy * dy + dz * dz};

            if (static_cast<int>(maxHeap.size()) < numConnections) {
                maxHeap.push({distanceSquared, i, j});
            } else if (distanceSquared < std::get<0>(maxHeap.top())) {
                maxHeap.pop();
                maxHeap.push({distanceSquared, i, j});
            }
        }
    }

    std::vector<Edge> closestEdges;
    closestEdges.reserve(maxHeap.size());
    while (!maxHeap.empty()) {
        closestEdges.push_back(maxHeap.top());
        maxHeap.pop();
    }

    std::sort(closestEdges.begin(), closestEdges.end(),
              [](const Edge& a, const Edge& b) { return std::get<0>(a) < std::get<0>(b); });

    std::vector<int> circuitParent(numBoxes, -1);
    for (const auto& edge : closestEdges) {
        int boxA{std::get<1>(edge)};
        int boxB{std::get<2>(edge)};

        mergeCircuits(circuitParent, boxA, boxB);
    }

    std::vector<std::int64_t> circuitSizes;
    for (int i{0}; i < numBoxes; i++)
        if (circuitParent[i] < 0)
            circuitSizes.push_back(-static_cast<std::int64_t>(circuitParent[i]));

    std::sort(circuitSizes.begin(), circuitSizes.end(), std::greater<std::int64_t>());

    std::int64_t result{1};
    for (int i{0}; i < 3; i++)
        result *= (i < static_cast<int>(circuitSizes.size()) ? circuitSizes[i] : 1);

    std::cout << result << '\n';
    return 0;
}