#ifndef WALLFOLLOWER_H
#define WALLFOLLOWER_H

#include "adjacencylist.h"
#include "solutionlist.h"

class WallFollower
{
public:

    enum E_HAND
    {
        E_LEFT_HAND,
        E_RIGHT_HAND
    };
    WallFollower(E_HAND hand);

    SolutionList solve(AdjacencyList &adjList);

private:
    E_HAND m_hand;
};

#endif // WALLFOLLOWER_H
