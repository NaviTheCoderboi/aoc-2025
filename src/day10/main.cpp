#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Machine {
    std::vector<std::vector<int>> buttons{};
    std::vector<int> joltage{};
};

Machine parseLine(const std::string& line) {
    Machine machine{};
    std::size_t i{0};

    while (i < line.size() && line[i] != ']')
        i++;
    i++;

    while (i < line.size()) {
        while (i < line.size() && line[i] != '(' && line[i] != '{')
            i++;
        if (i >= line.size() || line[i] == '{')
            break;

        i++;
        std::vector<int> button{};
        while (i < line.size() && line[i] != ')') {
            if (line[i] >= '0' && line[i] <= '9') {
                int num{};
                while (i < line.size() && line[i] >= '0' && line[i] <= '9') {
                    num = num * 10 + (line[i] - '0');
                    i++;
                }
                button.push_back(num);
            } else {
                i++;
            }
        }
        if (!button.empty())
            machine.buttons.push_back(button);
        if (i < line.size())
            i++;
    }

    while (i < line.size() && line[i] != '{')
        i++;
    if (i < line.size()) {
        i++;
        while (i < line.size() && line[i] != '}') {
            if (line[i] >= '0' && line[i] <= '9') {
                int num{};
                while (i < line.size() && line[i] >= '0' && line[i] <= '9') {
                    num = num * 10 + (line[i] - '0');
                    i++;
                }
                machine.joltage.push_back(num);
            } else {
                i++;
            }
        }
    }

    return machine;
}

int64_t solveMachine(const Machine& machine) {
    int numButtons{static_cast<int>(machine.buttons.size())};
    int numCounters{static_cast<int>(machine.joltage.size())};

    if (numCounters == 0)
        return 0;
    if (numButtons == 0) {
        for (int j : machine.joltage)
            if (j != 0)
                return -1;
        return 0;
    }

    std::vector<std::vector<double>> matrix(numCounters, std::vector<double>(numButtons + 1, 0.0));
    for (int b{0}; b < numButtons; b++) {
        for (int c : machine.buttons[b]) {
            if (c < numCounters)
                matrix[c][b] = 1.0;
        }
    }
    for (int c{0}; c < numCounters; c++)
        matrix[c][numButtons] = machine.joltage[c];

    std::vector<int> pivotCol(numCounters, -1);
    int rank{};

    for (int col{0}; col < numButtons && rank < numCounters; col++) {
        int pivotRow{-1};
        double maxVal = 1e-9;
        for (int r{rank}; r < numCounters; r++) {
            double val{matrix[r][col] < 0 ? -matrix[r][col] : matrix[r][col]};
            if (val > maxVal) {
                maxVal = val;
                pivotRow = r;
            }
        }
        if (pivotRow == -1)
            continue;

        std::swap(matrix[rank], matrix[pivotRow]);
        pivotCol[rank] = col;

        double pivotVal = matrix[rank][col];
        for (int c{0}; c <= numButtons; c++)
            matrix[rank][c] /= pivotVal;

        for (int r{0}; r < numCounters; r++) {
            if (r != rank && (matrix[r][col] > 1e-9 || matrix[r][col] < -1e-9)) {
                double factor{matrix[r][col]};
                for (int c{0}; c <= numButtons; c++)
                    matrix[r][c] -= factor * matrix[rank][c];
            }
        }
        rank++;
    }

    std::vector<bool> isPivot(numButtons, false);
    for (int r{0}; r < rank; r++)
        if (pivotCol[r] >= 0)
            isPivot[pivotCol[r]] = true;

    std::vector<int> freeVars{}, pivotVars{};
    for (int c{0}; c < numButtons; c++)
        (isPivot[c] ? pivotVars : freeVars).push_back(c);

    int numFree{static_cast<int>(freeVars.size())};

    int maxJoltage{*std::max_element(machine.joltage.begin(), machine.joltage.end())};
    std::vector<int> freeUpper(numFree, maxJoltage);
    for (int i{0}; i < numFree; i++) {
        int fv{freeVars[i]};
        for (int c : machine.buttons[fv]) {
            if (c < numCounters && machine.joltage[c] < freeUpper[i])
                freeUpper[i] = machine.joltage[c];
        }
    }

    int64_t bestTotal{std::numeric_limits<int64_t>::max()};
    std::vector<int> freeVals(numFree, 0);
    std::vector<int> allPresses(numButtons, 0);

    auto solve{[&](auto&& self, int idx, int64_t freeSum) -> void {
        if (freeSum >= bestTotal)
            return;
        if (idx == numFree) {
            for (int i{0}; i < numFree; i++)
                allPresses[freeVars[i]] = freeVals[i];

            int64_t total{freeSum};
            bool valid{true};

            for (int r{0}; r < rank && valid; r++) {
                double val{matrix[r][numButtons]};
                for (int i{0}; i < numFree; i++)
                    val -= matrix[r][freeVars[i]] * freeVals[i];

                int intVal{static_cast<int>(val + 0.5)};
                if (intVal < 0 || (val - intVal > 1e-6 || intVal - val > 1e-6))
                    valid = false;
                else {
                    allPresses[pivotCol[r]] = intVal;
                    total += intVal;
                }
                if (total >= bestTotal)
                    valid = false;
            }

            if (valid)
                bestTotal = total;
            return;
        }

        for (int v{0}; v <= freeUpper[idx]; v++) {
            freeVals[idx] = v;
            self(self, idx + 1, freeSum + v);
        }
    }};

    solve(solve, 0, 0);
    return bestTotal == std::numeric_limits<int64_t>::max() ? -1 : bestTotal;
}

int main() {
    std::ifstream infile("input.txt");
    std::string line{};
    int64_t totalPresses{};

    while (std::getline(infile, line)) {
        if (line.empty())
            continue;
        Machine machine = parseLine(line);
        int64_t presses = solveMachine(machine);
        if (presses >= 0)
            totalPresses += presses;
    }

    std::cout << totalPresses << "\n";
    return 0;
}
