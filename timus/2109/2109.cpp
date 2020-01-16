#include <ios>
#include <iostream>
#include <vector>


struct TGraph {
    std::vector<std::vector<int>> Parents;
    std::vector<int> Depth;
};


void ReadGraph(TGraph &g) {
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
    g.Parents.resize(n);
    g.Depth.resize(n);
    g.Parents[0].clear();
    g.Depth[0] = 0;
    for (size_t i = 0; i < queue.size(); ++i) {
        int current = queue[i];
        for (int child : links[current]) {
            if (child == 0)
                continue;
            if (!g.Parents[child].empty())
                continue;
            g.Parents[child].push_back(current);
            g.Depth[child] = g.Depth[current] + 1;
            queue.push_back(child);
        }
    }
    for (int current : queue) {
        for (size_t level = 0; level < g.Parents[current].size(); ++level) {
            if (level + 1 != g.Parents[current].size())
                throw 1;
            int parent = g.Parents[current][level];
            if (level >= g.Parents[parent].size())
                break;
            g.Parents[current].push_back(g.Parents[parent][level]);
        }
    }
}

int GetParent(int vertex, int level, const TGraph &g) {
    int pow2 = 0;
    while (level != 0) {
        if (level % 2 == 1)
            vertex = g.Parents[vertex][pow2];
        level >>= 1;
        ++pow2;
    }
    return vertex;
}

int CommonParent(int a, int b, const TGraph &g) {
    if (g.Depth[a] != g.Depth[b]) {
        if (g.Depth[a] > g.Depth[b])
            a = GetParent(a, g.Depth[a] - g.Depth[b], g);
        else
            b = GetParent(b, g.Depth[b] - g.Depth[a], g);
    }
    if (a == b)
        return a;
    for (;;) {
        size_t level = 0;
        for (; level < g.Parents[a].size() && g.Parents[a][level] != g.Parents[b][level]; ++level) {
        }
        if (level == 0)
            return g.Parents[a][0];
        a = g.Parents[a][level - 1];
        b = g.Parents[b][level - 1];
    }
}

int GreaterOrEqualPow2(int n) {
    for (int pow2 = 0; ; ++pow2)
        if ((1 << pow2) >= n)
            return pow2;
}

struct TIntervalTree {
    int Pow2 = 0;
    std::vector<int> Tree;
};

void BuildIntervalTree(const TGraph &g, TIntervalTree &tree) {
    tree.Pow2 = GreaterOrEqualPow2(g.Depth.size());
    tree.Tree.resize((1 << (tree.Pow2 + 1)) - 1, -1);
    int levelBegin = (1 << tree.Pow2) - 1, levelSize = (1 << tree.Pow2);
    for (int i = 0; i < g.Parents.size(); ++i)
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
                tree.Tree[levelBegin + i] = CommonParent(ans1, ans2, g);
        }
    }
}

int DoCommonParentForInterval(int index, int intervalBegin, int intervalSize, int a, int b, int bestKnownDepth, const TGraph &graph, const TIntervalTree &tree) {
    if (a > intervalBegin + intervalSize - 1)
        return -1;
    if (b < intervalBegin)
        return -1;
    int ansRoot = tree.Tree[index];
    if (ansRoot == -1)
        return -1;
    if (bestKnownDepth != -1 && graph.Depth[ansRoot] >= bestKnownDepth)
        return -1;
    if (intervalBegin >= a && intervalBegin + intervalSize - 1 <= b)
        return ansRoot;
    int ans1 = DoCommonParentForInterval(index * 2 + 1, intervalBegin, intervalSize / 2, a, b, bestKnownDepth, graph, tree);
    //if (ans1 != -1 && (bestKnownDepth == -1 || graph.Depth[ans1] < bestKnownDepth))
    //    bestKnownDepth = graph.Depth[ans1];
    int ans2 = DoCommonParentForInterval(index * 2 + 2, intervalBegin + intervalSize / 2, intervalSize / 2, a, b, bestKnownDepth, graph, tree);
    if (ans2 == -1)
        return ans1;
    if (ans1 == -1)
        return ans2;
    return CommonParent(ans1, ans2, graph);
}

int CommonParentForInterval(int a, int b, const TGraph &graph, const TIntervalTree &tree) {
    return DoCommonParentForInterval(0, 0, (1 << tree.Pow2), a, b, -1, graph, tree);
}

void ProcessQueries(const TGraph &graph, const TIntervalTree &tree) {
    int q;
    for (std::cin >> q; q > 0; --q) {
        int a, b;
        std::cin >> a >> b;
        --a;
        --b;
        std::cout << CommonParentForInterval(a, b, graph, tree) + 1 << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    TGraph graph;
    ReadGraph(graph);
    TIntervalTree tree;
    BuildIntervalTree(graph, tree);
    ProcessQueries(graph, tree);
    return 0;
}

