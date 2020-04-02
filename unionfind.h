#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

class UnionFind
{
public:
    explicit UnionFind(int n);
    ~UnionFind();

    bool connected(int p, int q) { return root(p) == root(q); }
    void connect(int p, int q);

    int nodeCount() { return m_nodeCount; }
    int connectionCount() { return m_connectionCount; }

private:
    int root(int p);

private:
    std::vector<int> m_index2parent;
    std::vector<int> m_index2treeSize;
    int m_nodeCount;
    int m_connectionCount;
};

#endif // UNIONFIND_H
