#include <ios>
#include <iostream>
#include <vector>


struct TGraph {
    std::vector<std::vector<int>> Parents;
    std::vector<int> Depth;
};

struct TIntervalTree {
    int Pow2 = 0;
    std::vector<int> Tree;
};

TGraph graph;
TIntervalTree tree;


int GreaterOrEqualPow2(int n) {
    for (int pow2 = 0; ; ++pow2)
        if ((1 << pow2) >= n)
            return pow2;
}

void ReadGraph() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> links(n);
    for (size_t i = 1; i < n; ++i) {
        int a, b;
        std::cin >> a >> b;
        links[a - 1].push_back(b - 1);
        links[b - 1].push_back(a - 1);
    }
    std::vector<int> queue;
    queue.reserve(n - 1);
    queue.push_back(0);
    graph.Parents.resize(n);
    int pow2 = GreaterOrEqualPow2(n);
    for (int i = 0; i < n; ++i)
        graph.Parents[i].reserve(pow2);
    graph.Depth.resize(n);
    graph.Parents[0].clear();
    graph.Depth[0] = 0;
    for (size_t i = 0; i < queue.size(); ++i) {
        int current = queue[i];
        for (int child : links[current]) {
            if (child == 0)
                continue;
            if (!graph.Parents[child].empty())
                continue;
            graph.Parents[child].push_back(current);
            graph.Depth[child] = graph.Depth[current] + 1;
            queue.push_back(child);
        }
    }
    for (int current : queue) {
        for (size_t level = 0; level < graph.Parents[current].size(); ++level) {
            if (level + 1 != graph.Parents[current].size())
                throw 1;
            int parent = graph.Parents[current][level];
            if (level >= graph.Parents[parent].size())
                break;
            graph.Parents[current].push_back(graph.Parents[parent][level]);
        }
    }
}

int GetParent(int vertex, int level) {
    int pow2 = 0;
    while (level != 0) {
        if (level % 2 == 1)
            vertex = graph.Parents[vertex][pow2];
        level >>= 1;
        ++pow2;
    }
    return vertex;
}

int FindCommonParent(int a, int b) {
    int l = 0, r = graph.Parents[a].size() - 1, result = -1;
    for (int i = l; i <= r; ++i)
        if (graph.Parents[a][i] == graph.Parents[b][i])
            return i - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (graph.Parents[a][m] != graph.Parents[b][m]) {
            result = m;
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return result;
}

int CommonParent(int a, int b) {
    int diff = graph.Depth[a] - graph.Depth[b];
    if (diff != 0) {
        if (diff > 0)
            a = GetParent(a, diff);
        else
            b = GetParent(b, -diff);
    }
    if (a == b)
        return a;
    for (;;) {
        int level = FindCommonParent(a, b);
        if (level == -1)
            return graph.Parents[a][0];
        a = graph.Parents[a][level];
        b = graph.Parents[b][level];
    }
}

void BuildIntervalTree() {
    tree.Pow2 = GreaterOrEqualPow2(graph.Depth.size());
    tree.Tree.resize((1 << (tree.Pow2 + 1)) - 1, -1);
    int levelBegin = (1 << tree.Pow2) - 1, levelSize = (1 << tree.Pow2);
    for (int i = 0; i < graph.Parents.size(); ++i)
        tree.Tree[levelBegin + i] = i;
    while(levelBegin != 0) {
        levelBegin = (levelBegin - 1) / 2;
        levelSize >>= 1;
        for (int i = 0; i < levelSize; ++i) {
            int idx1 = (levelBegin + i) * 2 + 1, idx2 = (levelBegin + i) * 2 + 2;
            int ans1 = tree.Tree[idx1], ans2 = tree.Tree[idx2];
            if (ans2 == -1)
                tree.Tree[levelBegin + i] = ans1;
            else if (ans1 == -1)
                tree.Tree[levelBegin + i] = ans2;
            else
                tree.Tree[levelBegin + i] = CommonParent(ans1, ans2);
        }
    }
}

void DoCommonParentForInterval(int index, int intervalBegin, int intervalSize, int a, int b, std::vector<int> &tops) {
    if (a > intervalBegin + intervalSize - 1)
        return;
    if (b < intervalBegin)
        return;
    int ansRoot = tree.Tree[index];
    if (ansRoot == -1)
        return;
    if (intervalBegin >= a && intervalBegin + intervalSize - 1 <= b) {
        tops.push_back(ansRoot);
        return;
    }
    DoCommonParentForInterval(index * 2 + 1, intervalBegin, intervalSize / 2, a, b, tops);
    DoCommonParentForInterval(index * 2 + 2, intervalBegin + intervalSize / 2, intervalSize / 2, a, b, tops);
}

int CommonParentForInterval(int a, int b) {
    static std::vector<int> tops;
    tops.reserve(100);
    tops.clear();
    DoCommonParentForInterval(0, 0, (1 << tree.Pow2), a, b, tops);
    int ans = -1;
    for (int v : tops)
        ans = (ans == -1 ? v : CommonParent(ans, v));
    return ans;
}

void ProcessQueries() {
    int q;
    for (std::cin >> q; q > 0; --q) {
        int a, b;
        std::cin >> a >> b;
        --a;
        --b;
        std::cout << CommonParentForInterval(a, b) + 1 << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    ReadGraph();
    BuildIntervalTree();
    ProcessQueries();
    return 0;
}


