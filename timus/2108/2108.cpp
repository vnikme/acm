#include <ios>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>


using ull = unsigned long long;

std::vector<ull> Binarize(const std::string &s, size_t n) {
    std::vector<ull> result;
    result.resize((s.size() + 63) / 64);
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '1')
            result[i / 64] |= (1ull << (i % 64));
    }
    return result;
}

void Or(std::vector<ull> &a, const std::vector<ull> &b) {
    auto it = a.begin();
    for (ull v : b) {
        (*it) |= v;
        ++it;
    }
}

bool IsSatisfied(const std::vector<ull> &desire, const std::vector<ull> &state) {
    auto it = state.begin();
    for (ull v : desire) {
        if ((v & (*it)) != v)
            return false;
        ++it;
    }
    return true;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t n, k;
    std::list<std::pair<std::vector<ull>, std::vector<ull>>> rules;
    for (std::cin >> n >> k; k > 0; --k) {
        std::string a, b;
        std::cin >> a >> b;
        rules.push_back(std::make_pair(Binarize(a, n), Binarize(b, n)));
    }
    std::string s;
    std::cin >> s;
    std::vector<ull> state = Binarize(s, n);
    for (;;) {
        bool found = false;
        for (auto it = rules.begin(); it != rules.end();) {
            if (IsSatisfied(it->first, state)) {
                found = true;
                Or(state, it->second);
                it = rules.erase(it);
            } else {
                ++it;
            }
        }
        if (!found)
            break;
    }
    for (size_t i = 0; i < n; ++i)
        std::cout << ((state[i / 64] & (1ull << (i % 64))) ? '1' : '0');
    std::cout << std::endl;
    return 0;
}
