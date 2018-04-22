#ifndef SOLUTIONLIST_H
#define SOLUTIONLIST_H

#include <vector>
#include <set>
#include <utility>

struct SolutionList
{
    SolutionList();

    void setupUseTrace();

    std::vector<std::pair<int, int>> m_vSolution;
    std::set<std::pair<int, int>> m_vAccessed;
    std::vector<std::pair<int, int>> m_vTrace;
};

#endif // SOLUTIONLIST_H
