#ifndef UNIONFIND_H
#define UNIONFIND_H


class UnionFind
{
public:
    UnionFind(int n);
    ~UnionFind();
    int Count() { return m_count; }
    bool Connected(int p, int q) { return find(p) == find(q); }
    void Union(int p, int q);
    int Connection() { return m_connection; }

private:
    int find(int p);

private:
    int *m_pid;
    int *m_psize;
    int m_count;
    int m_connection;
};

#endif // UNIONFIND_H
