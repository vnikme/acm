#include <ios>
#include <iostream>
#include <string>


bool IsFlipDigit(int d) {
    return d == 0 || d == 1 || d == 8 || d == 6 || d == 9;
}

bool IsFlipNumber(int n) {
    int h = n / 10, l = n % 10;
    return IsFlipDigit(h) && IsFlipDigit(l);
}

char FlipDigit(char d) {
    switch (d) {
        case '0': return '0';
        case '1': return '1';
        case '6': return '9';
        case '8': return '8';
        case '9': return '6';
    }
    throw 1;
}

std::string FlipNumber(int n) {
    const char result[] = {FlipDigit(static_cast<char>('0' + (n % 10))), FlipDigit(static_cast<char>('0' + (n / 10))), 0};
    return std::string(result);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    for (int i = 10; i <= 99; ) {
        for (int j = i; ; ++j) {
            if (!IsFlipNumber(j)) {
                i = j + 1;
                break;
            }
            if (j - i + 1 == n) {
                for (int k = j; k >= i; --k) {
                    if (k != j)
                        std::cout << ' ';
                    std::cout << FlipNumber(k);
                }
                std::cout << std::endl;
                return 0;
            }
        }
    }
    std::cout << "Glupenky Pierre" << std::endl;
    return 0;
}


