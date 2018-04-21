#include "deepfirstsearch.h"

#include <vector>
#include <algorithm>

using namespace std;

DeepFirstSearch::DeepFirstSearch(int width, int height)
    : m_iWidth(width)
    , m_iHeight(height)
{

}

AdjacencyList DeepFirstSearch::generate()
{
    AdjacencyList result(m_iWidth, m_iHeight);
    result.unlinkAllVertexes();

    vector<int> color(m_iWidth * m_iHeight, E_WHITE);
    vector<int> current;
    current.reserve(m_iWidth * m_iHeight);

    color[0] = E_GRAY;
    current.push_back(0);

    while(current.size() != 0)
    {
        int last = current.back();
        random_shuffle(result.m_vVertexesAllLinked[last].begin(), result.m_vVertexesAllLinked[last].end());

        auto iter = result.m_vVertexesAllLinked[last].cbegin();

        for (; iter != result.m_vVertexesAllLinked[last].cend(); iter++)
        {
            if (color[*iter] == E_WHITE)
            {
                color[*iter] = E_GRAY;
                result.connect(last, *iter);
                current.push_back(*iter);
                break;
            }
        }

        // all adjacent points are found
        if (iter == result.m_vVertexesAllLinked[last].cend())
        {
            current.pop_back();
            color[last] = E_BLACK;
        }
    }

    return result;
}
