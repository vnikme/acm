#include <algorithm>
#include <ios>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>


std::vector<int> ReadData() {
    size_t n;
    std::cin >> n;
    std::vector<int> result(n);
    for (auto &item : result)
        std::cin >> item;
    return result;
}

template<typename IterType>
bool IsAcsending(IterType begin, IterType end) {
    if (std::distance(begin, end) < 2)
        return true;
    for (auto it = begin + 1; it != end; ++it)
        if (*(it - 1) > *it)
            return false;
    return true;
}

void HeapAdd(std::vector<int> &heap, const std::vector<int> &data, int idx) {
    heap.push_back(idx);
    for (size_t k = heap.size() - 1; k != 0; ) {
        size_t parent = (k - 1) / 2;
        if (data[heap[k]] >= data[heap[parent]])
            break;
        std::swap(heap[k], heap[parent]);
        k = parent;
    }
}

bool Is1FixPossible(std::vector<int> data, size_t &a, size_t &b) {
    size_t n = data.size();
    std::vector<int> mins(n), heap;
    heap.reserve(n);
    for (size_t i = n; i != 0; --i) {
        HeapAdd(heap, data, i - 1);
        mins[i - 1] = heap.front();
    }
    size_t maxIdx = 0;
    for (size_t i = 0; i < n; ++i) {
        if (data[i] > data[maxIdx])
            maxIdx = i;
        if (data[maxIdx] > data[mins[i]]) {
            a = maxIdx;
            b = mins[i];
            std::swap(data[a], data[b]);
            return IsAcsending(data.begin(), data.end());
        }
    }
    throw 1;
}

std::string Work(std::vector<int> data) {
    if (IsAcsending(data.begin(), data.end()))
        return "Nothing to do here";
    if (IsAcsending(data.rbegin(), data.rend()))
        return "Nothing to do here";
    size_t a, b;
    if (Is1FixPossible(data, a, b)) {
        std::stringstream str;
        str << "Yes" << std::endl << a + 1 << ' ' << b + 1;
        return str.str();
    }
    std::reverse(data.begin(), data.end());
    if (Is1FixPossible(data, a, b)) {
        std::stringstream str;
        str << "Yes" << std::endl << data.size() - a << ' ' << data.size() - b;
        return str.str();
    }
    return "No hope";
}

void Output(const std::string &result) {
    std::cout << result << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Output(Work(ReadData()));
    return 0;
}

