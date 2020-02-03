#include <ios>
#include <iostream>
#include <iomanip>
#include <cmath>

constexpr double PI = 3.1415926535897;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::cout << std::fixed << std::setprecision(10) << (n == 1 ? 1 : (1.0 / sin(PI / n) + 1.0)) << std::endl;
    return 0;
}


