#include <ios>
#include <iostream>
#include <vector>


constexpr long long MAX_VALUE = 1000000000000l;

long long GCD(long long a, long long b) {
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

long long LCM(long long a, long long b) {
    long long d = GCD(a, b);
    if (static_cast<double>(a) * b / d > MAX_VALUE)
        return -1;
    return a / d * b;
}

int BinSearch(const std::vector<long long> &lcms, long long d, int a, int b) {
    if (b == -1)
        return -1;
    b += 1;
    int result = -1;
    while (a < b) {
        int t = a + (b - a) / 2;
        if (lcms[t] % d != 0) {
            result = t;
            a = t + 1;
        } else {
            b = t;
        }
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t n;
    std::cin >> n;
    std::vector<long long> d(n), lcms(n);
    std::vector<int> t(n);
    long long ans = 1;
    int upperBound = -1;
    for (int i = 0; i < n; ++i) {
        std::cin >> d[i] >> t[i];
        if (ans != -1 && t[i] == 1) {
            ans = LCM(ans, d[i]);
        }
        lcms[i] = ans;
        if (ans != -1)
            upperBound = i;
    }
    for (int i = 0; i < n; ++i) {
        if (i > upperBound)
            upperBound = -1;
        if (t[i] == 0) {
            upperBound = BinSearch(lcms, d[i], i, upperBound);
        }
        std::cout << (upperBound == -1 ? -1 : lcms[upperBound]) << std::endl;
    }
    //std::cout << std::endl;
    return 0;
}

