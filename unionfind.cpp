#include "unionfind.h"

using namespace std;

UnionFind::UnionFind(int n)
    : m_nodeCount(n)
    , m_connectionCount(0)
    , m_index2parent(n)
    , m_index2treeSize(n)
{
    for (int i = 0; i < n; i++)
    {
        m_index2parent[i] = i;
        m_index2treeSize[i] = 1;
    }
}

UnionFind::~UnionFind()
{

}

void UnionFind::connect(int p, int q)
{
    int i = root(p);
    int j = root(q);

    if (i == j)
    {
        return;
    }
    if (m_index2treeSize[i] < m_index2treeSize[j])
    {
        m_index2parent[i] = j;
        m_index2treeSize[j] += m_index2treeSize[i];
    }
    else
    {
        m_index2parent[j] = i;
        m_index2treeSize[i] += m_index2treeSize[j];
    }
    m_connectionCount++;
}

int UnionFind::root(int p)
{
    while (p != m_index2parent[p])
    {
        p = m_index2parent[p];
    }
    return p;
}
