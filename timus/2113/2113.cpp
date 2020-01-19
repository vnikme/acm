#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>


using TCell = std::pair<size_t, size_t>;

bool BFS(const std::vector<std::vector<size_t>> &grid, const std::vector<std::vector<int>> &water, const TCell &rabbitStart, int jumpHeight) {
    if (water[rabbitStart.first - 1][rabbitStart.second - 1] == -1)
        return true;
    size_t n = grid.size(), m = grid[0].size();
    std::vector<std::vector<int>> rabbit(n, std::vector<int>(m, -1));
    std::vector<TCell> queue;
    queue.reserve(n * m);
    rabbit[rabbitStart.first - 1][rabbitStart.second - 1] = 0;
    queue.push_back(std::make_pair(rabbitStart.first - 1, rabbitStart.second - 1));
    for (size_t i = 0; i < queue.size(); ++i) {
        TCell current = queue[i];
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (std::abs(dr) + std::abs(dc) != 1)
                    continue;
                int row = static_cast<int>(current.first) + dr;
                int col = static_cast<int>(current.second) + dc;
                if (row < 0 || row >= n || col < 0 || col >= m)
                    continue;
                if (grid[row][col] > grid[current.first][current.second] + jumpHeight)
                    continue;
                if (water[row][col] == -1)
                    return true;
                if (rabbit[row][col] != -1)
                    continue;
                if (rabbit[current.first][current.second] + 1 >= water[row][col])
                    continue;
                rabbit[row][col] = rabbit[current.first][current.second] + 1;
                queue.push_back(std::make_pair(row, col));
            }
        }
    }
    return false;
}

int Work() {
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t>> grid(n, std::vector<size_t>(m));
    size_t maxHeight = 0;
    for (auto &row : grid) {
        for (auto &cell : row) {
            std::cin >> cell;
            maxHeight = std::max(maxHeight, cell);
        }
    }
    std::vector<std::vector<int>> water(n, std::vector<int>(m, -1));
    TCell rabbitStart, waterStart;
    std::cin >> rabbitStart.first >> rabbitStart.second >> waterStart.first >> waterStart.second;
    std::vector<TCell> queue;
    queue.reserve(n * m);
    water[waterStart.first - 1][waterStart.second - 1] = 0;
    queue.push_back(std::make_pair(waterStart.first - 1, waterStart.second - 1));
    for (size_t i = 0; i < queue.size(); ++i) {
        TCell current = queue[i];
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (std::abs(dr) + std::abs(dc) != 1)
                    continue;
                int row = static_cast<int>(current.first) + dr;
                int col = static_cast<int>(current.second) + dc;
                if (row < 0 || row >= n || col < 0 || col >= m)
                    continue;
                if (water[row][col] != -1)
                    continue;
                if (grid[row][col] > grid[current.first][current.second])
                    continue;
                water[row][col] = water[current.first][current.second] + 1;
                queue.push_back(std::make_pair(row, col));
            }
        }
    }
    if (!BFS(grid, water, rabbitStart, maxHeight))
        return -1;
    int a = 0, b = maxHeight;
    while (a < b) {
        int t = a + (b - a) / 2;
        if (BFS(grid, water, rabbitStart, t))
            b = t;
        else
            a = t + 1;
    }
    return b;
}

void Output(int result) {
    std::cout << result << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Output(Work());
    return 0;
}

