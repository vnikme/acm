#include <ios>
#include <iostream>
#include <numeric>
#include <vector>


std::vector<int> ReadData() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<int> data;
    size_t n;
    std::cin >> n;
    data.reserve(n);
    for (; n > 0; --n) {
        data.emplace_back();
        std::cin >> data.back();
    }
    return data;
}

long long Solve(const std::vector<int> &data) {
    long long result = 0;
    long long sum = std::accumulate(data.begin(), data.end(), 0);
    for (long long a : data)
        result += (a * sum);
    return result;
}

void Output(long long result) {
    std::cout << result << std::endl;
}

int main() {
    Output(Solve(ReadData()));
    return 0;
}

