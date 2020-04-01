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

    static std::pair<int, int> makePair(const int &a, const int &b)
    {
        if (a < b)
        {
            return std::pair<int, int>(a, b);
        }
        else
        {
            return std::pair<int, int>(b, a);
        }
    }
};

#endif // SOLUTIONLIST_H
