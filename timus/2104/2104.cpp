#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>


using TStrip = std::pair<std::string, std::string>;

enum EWinner {
    W_ALICE,
    W_BOB,
    W_DRAW,
};

TStrip ReadData() {
    size_t n;
    TStrip s;
    std::cin >> n >> s.first >> s.second;
    return s;
}

EWinner GetWinner(const TStrip &s) {
    char aColor = s.first[0];
    for (char anotherColor : s.first)
        if (anotherColor != aColor)
            return W_DRAW;
    for (char anotherColor : s.second)
        if (anotherColor != aColor)
            return W_DRAW;
    return aColor == 'A' ? W_ALICE : W_BOB;
}

TStrip BendFirst(const TStrip &s) {
    TStrip result;
    size_t n = s.first.size() / 2;
    result.first = s.first.substr(0, n);
    result.second = s.first.substr(n, n);
    std::reverse(result.second.begin(), result.second.end());
    return result;
}

TStrip BendSecond(const TStrip &s) {
    TStrip result;
    size_t n = s.second.size() / 2;
    result.first = s.second.substr(0, n);
    result.second = s.second.substr(n, n);
    std::reverse(result.second.begin(), result.second.end());
    return result;
}

EWinner AliceMove(const TStrip &s);

namespace std {
    template <>
    struct hash<TStrip>
    {
        size_t operator()(const TStrip &s) const {
            return std::hash<std::string>()(s.first) ^ std::hash<std::string>()(s.second);
        }
    };
}

EWinner StoreInCache(std::unordered_map<TStrip, EWinner> &cache, TStrip strip, EWinner result) {
    cache[strip] = result;
    return result;
}

EWinner BobMove(const TStrip &s) {
    static std::unordered_map<TStrip, EWinner> cache;
    const auto it = cache.find(s);
    if (it != cache.end())
        return it->second;
    if (s.first.size() % 2 != 0)
        return StoreInCache(cache, s, GetWinner(s));
    TStrip first = BendFirst(s), second = BendSecond(s);
    EWinner winnerFirst = AliceMove(first), winnerSecond = AliceMove(second);
    if (winnerFirst == W_BOB || winnerSecond == W_BOB)
        return StoreInCache(cache, s, W_BOB);
    if (winnerFirst == W_DRAW || winnerSecond == W_DRAW)
        return StoreInCache(cache, s, W_DRAW);
    return StoreInCache(cache, s, W_ALICE);
}

EWinner AliceMove(const TStrip &s) {
    static std::unordered_map<TStrip, EWinner> cache;
    const auto it = cache.find(s);
    if (it != cache.end())
        return it->second;
    if (s.first.size() % 2 != 0)
        return StoreInCache(cache, s, GetWinner(s));
    TStrip first = BendFirst(s), second = BendSecond(s);
    EWinner winnerFirst = BobMove(first), winnerSecond = BobMove(second);
    if (winnerFirst == W_ALICE || winnerSecond == W_ALICE)
        return StoreInCache(cache, s, W_ALICE);
    if (winnerFirst == W_DRAW || winnerSecond == W_DRAW)
        return StoreInCache(cache, s, W_DRAW);
    return StoreInCache(cache, s, W_BOB);
}

void Output(EWinner winner) {
    std::cout << (winner == W_ALICE ? "Alice" : winner == W_BOB ? "Bob" : "Draw") << std::endl;
}

int main() {
    Output(AliceMove(ReadData()));
    return 0;
}

