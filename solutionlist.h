#ifndef SOLUTIONLIST_H
#define SOLUTIONLIST_H

#include <vector>
#include <set>
#include <utility>

struct SolutionList
{
    SolutionList();

    void setupUseTrace();

    std::vector<std::pair<int, int>> m_solution;
    std::set<std::pair<int, int>> m_accessed;
    std::vector<std::pair<int, int>> m_trace;
};

#endif // SOLUTIONLIST_H
