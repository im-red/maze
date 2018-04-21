#include "unionfind.h"

using namespace std;

UnionFind::UnionFind(int n)
    : m_count(n)
    , m_connection(0)
{
    m_vPoints = vector<int>(n);
    m_vTreeSize = vector<int>(n);

    for (int i = 0; i < n; i++)
    {
        m_vPoints[i] = i;
        m_vTreeSize[i] = 1;
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
    if (m_vTreeSize[i] < m_vTreeSize[j])
    {
        m_vPoints[i] = j;
        m_vTreeSize[j] += m_vTreeSize[i];
    }
    else
    {
        m_vPoints[j] = i;
        m_vTreeSize[i] += m_vTreeSize[j];
    }
    m_connection++;
}

int UnionFind::root(int p)
{
    while (p != m_vPoints[p])
    {
        p = m_vPoints[p];
    }
    return p;
}
