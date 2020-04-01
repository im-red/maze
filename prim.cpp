#include "prim.h"

#include <vector>
#include <set>
#include <random>
#include <algorithm>
#include <utility>
#include <ctime>

using namespace std;

Prim::Prim(int row, int column)
    : m_row(row)
    , m_column(column)
{

}

AdjacencyList Prim::generate()
{
    AdjacencyList result(m_row, m_column);
    result.unlinkAllNodes();

    vector<bool> linked(m_row * m_column, false);
    linked[0] = true;

    set<pair<int ,int>> paths;
    paths.insert(pair<int, int>(0, 1));
    paths.insert(pair<int, int>(0, m_column));

    static default_random_engine e(time(nullptr));

    while (!paths.empty())
    {
        // random select a path in paths
        int pos = e() % paths.size();
        auto iter = paths.begin();
        while (pos--)
        {
            iter++;
        }

        // connect the two node of path
        result.connect(iter->first, iter->second);

        // get the node not in linked
        int current = 0;
        if (!linked[iter->first])
        {
            current = iter->first;
        }
        else
        {
            current = iter->second;
        }

        // add the node to linked
        linked[current] = true;

        // add all not accessed path to paths, and delete all invalid path from paths
        for (auto i : result.m_nodesAllLinked[current])
        {
            pair<int, int> currentPath;
            if (i > current)
            {
                currentPath = pair<int, int>(current, i);
            }
            else
            {
                currentPath = pair<int, int>(i, current);
            }

            if (!linked[i])
            {
                paths.insert(currentPath);
            }
            else
            {
                paths.erase(paths.find(currentPath));
            }
        }
    }

    return result;
}
