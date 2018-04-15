#include "unionfind.h"

UnionFind::UnionFind(int n)
    : m_count(n),
      m_connection(0),
      m_pid(new int[n]),
      m_psize(new int[n])
{
    for (int i = 0; i < n; i++)
    {
        m_pid[i] = i;
        m_psize[i] = 1;
    }
}

UnionFind::~UnionFind()
{
    delete [] m_pid;
    delete [] m_psize;
}

void UnionFind::Union(int p, int q)
{
    int i = find(p);
    int j = find(q);
    if (i == j)
    {
        return;
    }
    if (m_psize[i] < m_psize[j])
    {
        m_pid[i] = j;
        m_psize[j] += m_psize[i];
    }
    else
    {
        m_pid[j] = i;
        m_psize[i] += m_psize[j];
    }
    m_connection++;
}

int UnionFind::find(int p)
{
    while (p != m_pid[p])
    {
        p = m_pid[p];
    }
    return p;
}
