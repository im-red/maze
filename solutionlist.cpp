#include "solutionlist.h"

using namespace std;

SolutionList::SolutionList()
{

}

void SolutionList::setupUseTrace()
{
    m_vSolution.clear();
    m_vAccessed.clear();
    for (auto &&edge : m_vTrace)
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
        if (m_vAccessed.count(orderedEdge) == 1)
        {
            m_vSolution.erase(m_vSolution.end() - 1);
        }
        else
        {
            m_vAccessed.insert(orderedEdge);
            m_vSolution.push_back(orderedEdge);
        }
    }
}
