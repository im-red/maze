#include "kruskal.h"
#include "unionfind.h"

#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

Kruskal::Kruskal(int width, int height)
    : m_iWidth(width)
    , m_iHeight(height)
{

}

AdjacencyList Kruskal::generate()
{
    AdjacencyList result(m_iWidth, m_iHeight);
    result.unlinkAllVertexes();

    UnionFind uf(m_iWidth * m_iHeight);

    vector<pair<int, int>> edges;
    for (int i = 0; i < m_iWidth * m_iHeight; i++)
    {
        for (auto iter : result.m_vVertexesAllLinked[i])
        {
            // avoid duplicate edge
            if (i > iter)
            {
                edges.push_back(pair<int, int>(i, iter));
            }
        }
    }
    random_shuffle(edges.begin(), edges.end());
    for (auto iter : edges)
    {
        if(!uf.connected(iter.first, iter.second))
        {
            uf.connect(iter.first, iter.second);
            result.connect(iter.first, iter.second);
        }
    }
    return result;
}
