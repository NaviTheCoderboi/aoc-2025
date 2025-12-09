#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

int main() {
    std::ifstream file("input.txt");
    if (!file || !file.good()) {
        std::cerr << "Failed to open input file\n";
        return 1;
    }

    std::vector<std::pair<int, int>> redTiles{};
    redTiles.reserve(496);

    std::string line{};
    while (std::getline(file, line)) {
        int commaPos{static_cast<int>(line.find(','))};

        int x{std::stoi(line.substr(0, commaPos))};
        int y{std::stoi(line.substr(commaPos + 1))};

        redTiles.emplace_back(x, y);
    }

    int n{static_cast<int>(redTiles.size())};

    std::set<int> xSet{};
    std::set<int> ySet{};
    for (auto [x, y] : redTiles) {
        xSet.insert(x);
        ySet.insert(y);
    }

    std::map<int, int> xToIndex{};
    std::map<int, int> yToIndex{};
    int idx{1};
    for (int x : xSet)
        xToIndex[x] = idx++;
    idx = 1;
    for (int y : ySet)
        yToIndex[y] = idx++;

    int gridWidth{static_cast<int>(xToIndex.size()) + 2};
    int gridHeight{static_cast<int>(yToIndex.size()) + 2};

    std::vector<std::vector<int>> grid(gridWidth, std::vector<int>(gridHeight, -1));
    for (int i{0}; i < n; i++) {
        auto [prevXOrig, prevYOrig] = redTiles[i - 1 < 0 ? n - 1 : i - 1];
        auto [currXOrig, currYOrig] = redTiles[i];

        int prevX{xToIndex[prevXOrig]};
        int prevY{yToIndex[prevYOrig]};
        int currX{xToIndex[currXOrig]};
        int currY{yToIndex[currYOrig]};

        if (prevX == currX) {
            for (int y{std::min(prevY, currY)}; y <= std::max(prevY, currY); y++)
                grid[currX][y] = 1;
        } else {
            for (int x{std::min(prevX, currX)}; x <= std::max(prevX, currX); x++)
                grid[x][currY] = 1;
        }
    }

    std::stack<std::pair<int, int>> stk{};
    stk.push({0, 0});
    grid[0][0] = 0;

    int dx[]{0, 0, 1, -1};
    int dy[]{1, -1, 0, 0};

    while (!stk.empty()) {
        auto [x, y]{stk.top()};
        stk.pop();
        for (int i{0}; i < 4; i++) {
            int nx{x + dx[i]}, ny{y + dy[i]};

            if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight && grid[nx][ny] == -1) {
                grid[nx][ny] = 0;
                stk.push({nx, ny});
            }
        }
    }

    for (int x{0}; x < gridWidth; x++)
        for (int y{0}; y < gridHeight; y++)
            if (grid[x][y] == -1)
                grid[x][y] = 1;

    std::vector<std::vector<int>> prefixSum(gridWidth + 1, std::vector<int>(gridHeight + 1, 0));
    for (int x{0}; x < gridWidth; x++)
        for (int y{0}; y < gridHeight; y++)
            prefixSum[x + 1][y + 1] = grid[x][y] + prefixSum[x][y + 1] + prefixSum[x + 1][y] - prefixSum[x][y];

    auto getRectSum{[&](int a, int b, int c, int d) {
        return prefixSum[c][d] - prefixSum[a][d] - prefixSum[c][b] + prefixSum[a][b];
    }};

    uint64_t maxArea{0};
    for (int i{0}; i < n - 1; i++) {
        auto [x1, y1] = redTiles[i];
        for (int j{i + 1}; j < n; j++) {
            auto [x2, y2] = redTiles[j];

            int minX{std::min(x1, x2)}, minY{std::min(y1, y2)};
            int maxX{std::max(x1, x2)}, maxY{std::max(y1, y2)};

            int minXIdx{xToIndex[minX]}, minYIdx{yToIndex[minY]};
            int maxXIdx{xToIndex[maxX]}, maxYIdx{yToIndex[maxY]};

            if (getRectSum(minXIdx, minYIdx, maxXIdx + 1, maxYIdx + 1) ==
                (maxXIdx - minXIdx + 1) * (maxYIdx - minYIdx + 1)) {
                uint64_t area{static_cast<uint64_t>(maxX - minX + 1) * static_cast<uint64_t>(maxY - minY + 1)};
                if (area > maxArea)
                    maxArea = area;
            }
        }
    }

    std::cout << maxArea << "\n";
    return 0;
}
