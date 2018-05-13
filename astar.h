#ifndef ASTAR_H
#define ASTAR_H

#include "adjacencylist.h"
#include "solutionlist.h"

struct AStarNode
{
    AStarNode(int id = -1) : m_iId(id), m_iF(0), m_iG(0), m_iH(0) {}
    int m_iId;
    int m_iF;
    int m_iG;
    int m_iH;
};

class AStar
{
public:
    enum E_HFUNC
    {
        E_MANHATTAN,
        E_EUCLIDIAN,
        E_0
    };
    AStar(E_HFUNC func);

    SolutionList solve(AdjacencyList &adjList);
private:
    static int manhattanDistance(int p, int q, int width);
    static int euclidianDistance(int p, int q, int width);
    static int zeroDistance(int p, int q, int width);
    E_HFUNC m_hType;
};

#endif // ASTAR_H
