#include <ios>
#include <iostream>
#include <cmath>


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    long long L, T, Va, Vb, s = 0;
    size_t n;
    std::cin >> L >> T >> Va >> Vb >> n;
    s = (Va + Vb) * T;
    for (; n > 0; --n) {
        int who;
        long long start, duration;
        std::cin >> who >> start >> duration;
        s -= ((who == 1 ? Va : Vb) * duration);
    }
    std::cout << s / L << std::endl;
    return 0;
}

