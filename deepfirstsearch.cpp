#include "deepfirstsearch.h"

#include <vector>
#include <algorithm>

using namespace std;

DeepFirstSearch::DeepFirstSearch(int row, int column)
    : m_row(row)
    , m_column(column)
{

}

AdjacencyList DeepFirstSearch::generate()
{
    AdjacencyList result(m_row, m_column);
    result.unlinkAllNodes();

    vector<int> color(m_row * m_column, White);
    vector<int> current;
    current.reserve(m_row * m_column);

    color[0] = Gray;
    current.push_back(0);

    while(current.size() != 0)
    {
        int last = current.back();
        random_shuffle(result.m_nodesAllLinked[last].begin(), result.m_nodesAllLinked[last].end());

        auto iter = result.m_nodesAllLinked[last].cbegin();

        for (; iter != result.m_nodesAllLinked[last].cend(); iter++)
        {
            if (color[*iter] == White)
            {
                color[*iter] = Gray;
                result.connect(last, *iter);
                current.push_back(*iter);
                break;
            }
        }

        // all adjacent points are found
        if (iter == result.m_nodesAllLinked[last].cend())
        {
            current.pop_back();
            color[last] = Black;
        }
    }

    return result;
}
