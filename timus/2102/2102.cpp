#include <iostream>
#include <cmath>


long long ReadData() {
    long long n;
    std::cin >> n;
    return n;
}

size_t BruteForce(long long n) {
    size_t result = 0;
    for (long long i = 2; i * i <= n; ++i) {
        while (n % i == 0) {
            ++result;
            n /= i;
        }
    }
    if (n != 1)
        ++result;
    return result;
}

bool Work(long long n) {
    size_t targetPower = 20;
    for (long long i = 2; n > 1 && targetPower > 2; ++i) {
        if (log(n) / log(i) < targetPower - 1)
            return false;
        while (n % i == 0) {
            --targetPower;
            n /= i;
        }
    }
    return BruteForce(n) == targetPower;
}

void Output(bool result) {
    std::cout << (result ? "Yes" : "No") << std::endl;
}

int main() {
    Output(Work(ReadData()));
    return 0;
}

