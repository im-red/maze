#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

class UnionFind
{
public:
    UnionFind(int n);
    ~UnionFind();

    bool connected(int p, int q) { return root(p) == root(q); }
    void connect(int p, int q);

    int pointCount() { return m_pointCount; }
    int connectionCount() { return m_connectionCount; }

private:
    int root(int p);

private:
    std::vector<int> m_index2parent;
    std::vector<int> m_index2treeSize;
    int m_pointCount;
    int m_connectionCount;
};

#endif // UNIONFIND_H
