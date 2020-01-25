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
    double tmp = static_cast<double>(a) / d * b;
    if (tmp > MAX_VALUE)
        return -1;
    return static_cast<long long>(tmp);
}

const long long *BinSearch(const long long *a, const long long *b, long long d) {
    ++b;
    const long long *result = nullptr;
    while (a < b) {
        const long long *t = a + std::distance(a, b) / 2;
        if (*t % d != 0) {
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
    const long long *upperBound = nullptr;
    for (int i = 0; i < n; ++i) {
        std::cin >> d[i] >> t[i];
        if (ans != -1 && t[i] == 1) {
            ans = LCM(ans, d[i]);
        }
        lcms[i] = ans;
        if (ans != -1)
            upperBound = &lcms[i];
    }
    const long long *di = &d[0], *li = &lcms[0];
    const int *ti = &t[0];
    for (int i = 0; i < n; ++i, ++di, ++li, ++ti) {
        if (upperBound != nullptr && li > upperBound)
            upperBound = nullptr;
        if (upperBound != nullptr && *ti == 0)
            upperBound = BinSearch(li, upperBound, *di);
        std::cout << (upperBound == nullptr ? -1 : *upperBound) << '\n';
    }
    return 0;
}

