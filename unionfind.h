#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

class UnionFind
{
public:
    UnionFind(int n);
    ~UnionFind();
    int pointNum() { return m_count; }
    bool connected(int p, int q) { return root(p) == root(q); }
    void connect(int p, int q);
    int connectionNum() { return m_connection; }

private:
    int root(int p);

private:
    std::vector<int> m_vPoints;
    std::vector<int> m_vTreeSize;
    int m_count;
    int m_connection;
};

#endif // UNIONFIND_H
