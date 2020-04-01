#include "kruskal.h"
#include "unionfind.h"

#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

Kruskal::Kruskal(int row, int column)
    : m_row(row)
    , m_column(column)
{

}

AdjacencyList Kruskal::generate()
{
    AdjacencyList result(m_row, m_column);
    result.unlinkAllNodes();

    UnionFind uf(m_row * m_column);

    vector<pair<int, int>> edges;
    for (int i = 0; i < m_row * m_column; i++)
    {
        for (auto iter : result.m_nodesAllLinked[i])
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
