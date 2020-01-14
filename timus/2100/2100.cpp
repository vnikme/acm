#include <iostream>
#include <string>


int main() {
    size_t n, pluses = 0;
    std::string s;
    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> s;
        if (s.find('+') != s.npos)
            ++pluses;
    }
    n = n + pluses + 2;
    if (n == 13)
        ++n;
    std::cout << n * 100 << std::endl;
    return 0;
}

