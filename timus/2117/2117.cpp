#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>


std::vector<int> Eratosphen(int n) {
    std::vector<bool> mask(n + 1, true);
    std::vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (!mask[i])
            continue;
        primes.push_back(i);
        for (int j = 2 * i; j <= n; j += i)
            mask[j] = false;
    }
    return primes;
}

long long Sqrt(long long n) {
    long long t = static_cast<long long>(sqrt(n));
    if (t * t == n)
        return t;
    return t + 1;
}

long long TripplesFast(long long n) {
    if (n <= 1)
        return 1;
    std::unordered_map<long long, int> primes;
    for (int k : Eratosphen(1000000)) {
        int exponent = 0;
        while (n % k == 0) {
            n /= k;
            ++exponent;
        }
        if (exponent / 2 > 0)
            primes[k] = exponent / 2;
        if (n == 1)
            break;
    }
    if (n != 1) {
        long long k = Sqrt(n);
        if (k * k == n)
            primes[k] = 1;
    }
    long long odd = 1;
    int exp2 = 0;
    for (const auto &item : primes) {
        if (item.first != 2) {
            for (int j = 0; j < item.second; ++j)
                odd *= item.first;
        } else {
            exp2 = item.second;
        }
    }
    odd = (odd + 1) / 2;
    long long result = odd;
    if (exp2 == 0)
        return result;
    result *= 2;
    long long add = odd * 2 - 1;
    for (int j = 1; j < exp2; ++j) {
        result += add;
        add *= 2;
    }
    return result;
}

int main() {
    long long n;
    std::cin >> n;
    std::cout << TripplesFast(n) << std::endl;
}

