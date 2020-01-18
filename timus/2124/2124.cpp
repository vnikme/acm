#include <iostream>


int CyclicOrder(int a, int p) {
    int order = 1;
    //std::cout << std::endl << 1;
    for (int x = a; x != 1; ++order) {
        //std::cout << ' ' << x;
        x = (x * a) % p;
    }
    //std::cout << std::endl;
    return order;
}

int Log(int i, int a, int p) {
    return 1;
    int pow = 0, x = 1;
    for (; x != i; ++pow) {
        x = (x * a) % p;
    }
    return pow;
}

int main() {
    int p, a;
    std::cin >> p >> a;
    for (int i = 1; i < p; ++i)
        std::cout << i << ' ' << CyclicOrder(i, p) << ' ' << Log(i, a, p) << std::endl;
    return 0;
}

