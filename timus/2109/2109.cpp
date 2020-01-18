#include <algorithm>
#include <cmath>
#include <ios>
#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>


std::vector<std::vector<int>> Parents;
std::vector<int> Depth;
int Pow2 = 0;
std::vector<int> Tree;


int GreaterOrEqualPow2(int n) {
    for (int pow2 = 0; ; ++pow2)
        if ((1 << pow2) >= n)
            return pow2;
}

void ReadGraph() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> links(n);
    for (size_t i = 0; i < n; ++i)
        links.reserve(100);
    for (size_t i = 1; i < n; ++i) {
        int a, b;
        std::cin >> a >> b;
        links[a - 1].push_back(b - 1);
        links[b - 1].push_back(a - 1);
    }
    std::vector<int> queue;
    queue.reserve(n);
    queue.push_back(0);
    Parents.resize(n);
    Pow2 = GreaterOrEqualPow2(n);
    for (int i = 0; i < n; ++i)
        Parents[i].reserve(Pow2 + 1);
    Depth.resize(n);
    Depth[0] = 0;
    for (size_t i = 0; i < queue.size(); ++i) {
        int current = queue[i];
        for (int child : links[current]) {
            if (child == 0)
                continue;
            if (!Parents[child].empty())
                continue;
            Parents[child].push_back(current);
            Depth[child] = Depth[current] + 1;
            queue.push_back(child);
        }
    }
    for (int current : queue) {
        auto &currentParents = Parents[current];
        for (size_t level = 0; level < currentParents.size(); ++level) {
            if (level + 1 != currentParents.size())
                throw 1;
            int parent = currentParents[level];
            if (level >= Parents[parent].size())
                break;
            currentParents.push_back(Parents[parent][level]);
        }
    }
}

int GetParent(int vertex, int level) {
    int pow2 = 0;
    while (level != 0) {
        if (level % 2 == 1)
            vertex = Parents[vertex][pow2];
        level >>= 1;
        ++pow2;
    }
    return vertex;
}

int FindCommonParent(int a, int b) {
    const auto &pa = Parents[a];
    const auto &pb = Parents[b];
    int l = 0, r = pa.size() - 1, result = -1;
    for (; r >= 0 && pa[r] == pb[r]; --r);
    return r;
    while (l <= r) {
        int m = (l + r) / 2;
        if (pa[m] != pb[m]) {
            result = m;
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return result;
}


namespace std {
    template <>
    struct hash<std::pair<int, int>>
    {
        size_t operator()(const std::pair<int, int> &p) const {
            return p.first ^ (p.second + 200000);
        }
    };
}


std::unordered_map<std::pair<int, int>, int> Cache;

int CommonParent(int a, int b) {
    auto p = std::make_pair(a, b);
    auto it = Cache.find(p);
    if (it != Cache.end())
        return it->second;
    int diff = Depth[a] - Depth[b];
    if (diff != 0) {
        if (diff > 0)
            a = GetParent(a, diff);
        else
            b = GetParent(b, -diff);
    }
    if (a == b) {
        Cache[p] = a;
        return a;
    }
    for (;;) {
        int level = FindCommonParent(a, b);
        if (level == -1) {
            Cache[p] = Parents[a][0];
            return Parents[a][0];
        }
        a = Parents[a][level];
        b = Parents[b][level];
    }
}

void BuildIntervalTree() {
    Tree.resize((1 << (Pow2 + 1)) - 1, -1);
    int levelBegin = (1 << Pow2) - 1, levelSize = (1 << Pow2);
    for (int i = 0; i < Parents.size(); ++i)
        Tree[levelBegin + i] = i;
    while(levelBegin != 0) {
        levelBegin = (levelBegin - 1) / 2;
        levelSize >>= 1;
        for (int i = levelBegin; i < levelBegin + levelSize; ++i) {
            int idx1 = i * 2 + 1, idx2 = i * 2 + 2;
            int ans1 = Tree[idx1], ans2 = Tree[idx2];
            if (ans2 == -1)
                Tree[i] = ans1;
            else if (ans1 == -1)
                Tree[i] = ans2;
            else
                Tree[i] = CommonParent(ans1, ans2);
        }
    }
}

void DoCommonParentForInterval(int index, int intervalBegin, int intervalSize, int a, int b, std::vector<int> &tops) {
    if (a > intervalBegin + intervalSize - 1)
        return;
    if (b < intervalBegin)
        return;
    int ansRoot = Tree[index];
    if (ansRoot == -1)
        return;
    if (a <= intervalBegin && intervalBegin + intervalSize - 1 <= b)  {
        tops.push_back(ansRoot);
        return;
    }
    DoCommonParentForInterval(index * 2 + 1, intervalBegin, intervalSize / 2, a, b, tops);
    DoCommonParentForInterval(index * 2 + 2, intervalBegin + intervalSize / 2, intervalSize / 2, a, b, tops);
}

int CommonParentForInterval(int a, int b) {
    static std::vector<int> tops;
    tops.reserve(50);
    DoCommonParentForInterval(0, 0, (1 << Pow2), a, b, tops);
    int ans = -1;
    for (int v : tops)
        ans = (ans == -1 ? v : CommonParent(ans, v));
    tops.clear();
    return ans;
}

void ProcessQueries() {
    int q;
    for (std::cin >> q; q > 0; --q) {
        int a, b;
        std::cin >> a >> b;
        std::cout << CommonParentForInterval(a - 1, b - 1) + 1 << '\n';
    }
}

int main() {
    Cache.reserve(10000000);
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    ReadGraph();
    BuildIntervalTree();
    ProcessQueries();
    return 0;
}

