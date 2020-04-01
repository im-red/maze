#include "solutionlist.h"

using namespace std;

SolutionList::SolutionList()
{

}

void SolutionList::setupUseTrace()
{
    m_solution.clear();
    m_accessed.clear();
    for (auto &&edge : m_trace)
    {
        pair<int, int> orderedEdge;
        if (edge.first > edge.second)
        {
            orderedEdge = pair<int, int>(edge.second, edge.first);
        }
        else
        {
            orderedEdge = edge;
        }
        if (m_accessed.count(orderedEdge) == 1)
        {
            m_solution.erase(m_solution.end() - 1);
        }
        else
        {
            m_accessed.insert(orderedEdge);
            m_solution.push_back(orderedEdge);
        }
    }
}
