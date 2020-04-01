#ifndef WALLFOLLOWER_H
#define WALLFOLLOWER_H

#include "adjacencylist.h"
#include "solutionlist.h"

class WallFollower
{
public:
    enum Hand
    {
        LeftHand,
        RightHand
    };
    explicit WallFollower(Hand hand);

    SolutionList solve(AdjacencyList &adjList);

private:
    Hand m_hand;
};

#endif // WALLFOLLOWER_H
