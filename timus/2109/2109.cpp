#include <ios>
#include <iostream>
#include <vector>


struct TGraph {
    std::vector<std::vector<int>> Parents;
    std::vector<int> Depth;
};


TGraph ReadGraph() {
    size_t n;
    std::cin >> n;
    std::vector<std::vector<int>> links(n);
    for (size_t i = 1; i < n; ++i) {
        int a, b;
        std::cin >> a >> b;
        links[a - 1].push_back(b - 1);
        links[b - 1].push_back(a - 1);
    }
    std::vector<int> queue;
    queue.push_back(0);
    TGraph g;
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
    return g;
}

int GetParent(int vertex, int level, const TGraph &g) {
    int pow2 = 0;
    while (level != 0) {
        //std::cout << "Get parent: " << vertex << ' ' << level << ' ' << pow2 << std::endl;
        if (level % 2 == 1)
            vertex = g.Parents[vertex][pow2];
        level >>= 1;
        ++pow2;
    }
    return vertex;
}

int CommonParent(int a, int b, const TGraph &g) {
    //std::cout << "Depths: " << a << ' ' << b << ' ' << g.Depth[a] << ' ' << g.Depth[b] << std::endl;
    if (g.Depth[a] != g.Depth[b]) {
        if (g.Depth[a] > g.Depth[b])
            a = GetParent(a, g.Depth[a] - g.Depth[b], g);
        else
            b = GetParent(b, g.Depth[b] - g.Depth[a], g);
    }
    //std::cout << "Equals: " << a << ' ' << b << std::endl;
    if (a == b)
        return a;
    for (;;) {
        size_t level = 0;
        //std::cout << "Searching for parent: " << a << ' ' << b << ' ' << g.Depth[a] << ' ' << g.Depth[b] << ' ' << g.Parents[a].size() << ' ' << g.Parents[b].size() << std::endl;
        for (; level < g.Parents[a].size() && g.Parents[a][level] != g.Parents[b][level]; ++level) {
        }
        if (level == 0)
            return g.Parents[a][0];
        //std::cout << "Found parent at level " << level << ' ' << g.Parents[a][level - 1] << ' ' << g.Parents[b][level - 1] << ' ' << g.Parents[a][level] << std::endl;
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

TIntervalTree BuildIntervalTree(const TGraph &g) {
    /*
        Base level has size 2^n
        T[k][m] = Result(k, k + 2^m)
        T[0] = Result(0, 2^n)
        T[1], T[2] = 
    */
    TIntervalTree tree;
    tree.Pow2 = GreaterOrEqualPow2(g.Depth.size());
    tree.Tree.resize((1 << (tree.Pow2 + 1)) - 1, -1);
    int levelBegin = (1 << tree.Pow2) - 1, levelSize = (1 << tree.Pow2);
    //std::cout << tree.Pow2 << ' ' << levelBegin << ' ' << levelSize << ' ' << tree.Tree.size() << ' ' << g.Parents.size() << std::endl;
    for (int i = 0; i < g.Parents.size(); ++i)
        tree.Tree[levelBegin + i] = i;
    while(levelBegin != 0) {
        levelBegin = (levelBegin - 1) / 2;
        levelSize >>= 1;
        //std::cout << levelBegin << ' ' << levelSize << std::endl;
        for (int i = 0; i < levelSize; ++i) {
            int idx1 = (levelBegin + i) * 2 + 1, idx2 = (levelBegin + i) * 2 + 2;
            int ans1 = tree.Tree[idx1], ans2 = tree.Tree[idx2];
            //std::cout << "Idx/ans: " << idx1 << ' ' << idx2 << ' ' << ans1 << ' ' << ans2 << std::endl;
            if (ans2 == -1)
                tree.Tree[levelBegin + i] = ans1;
            else
                tree.Tree[levelBegin + i] = CommonParent(ans1, ans2, g);
            //std::cout << "Success" << std::endl;
        }
    }
    return tree;
}

void ProcessQueries(const TGraph &g) {
    size_t q;
    for (std::cin >> q; q > 0;--q) {
        int a, b;
        std::cin >> a >> b;
        //std::cout << "Input is: " << a << ' ' << b << std::endl;
        --a;
        --b;
        int answer = a, answerDepth = g.Depth[a];
        for (int i = a; i < b; ++i) {
            int parent = CommonParent(i, i + 1, g);
            if (g.Depth[parent] < answerDepth) {
                answer = parent;
                answerDepth = g.Depth[parent];
            }
        }
        std::cout << answer + 1 << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    auto g = ReadGraph();
    auto tree = BuildIntervalTree(g);
    for (int i = 0; i < tree.Tree.size(); ++i)
        std::cout << tree.Tree[i] << ' ';
    std::cout << std::endl;
    ProcessQueries(g);
    return 0;
}

