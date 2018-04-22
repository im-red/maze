#include "wallfollower.h"

#include <map>

using namespace std;

WallFollower::WallFollower(WallFollower::E_HAND hand)
    : m_hand(hand)
{

}

SolutionList WallFollower::solve(AdjacencyList &adjList)
{
    enum E_DIRECTION
    {
        E_DOWN = 0, // 00
        E_LEFT = 1, // 01
        E_UP = 2,   // 10
        E_RIGHT = 3 // 11
    };

    map<E_DIRECTION, int> delta;
    delta[E_UP] = -adjList.m_iWidth;
    delta[E_DOWN] = adjList.m_iWidth;
    delta[E_LEFT] = -1;
    delta[E_RIGHT] = 1;

    int deadDelta = 0;
    int turnDelta = 0;

    if (m_hand == E_RIGHT_HAND)
    {
        turnDelta = 1;
        deadDelta = -1;
    }
    else
    {
        turnDelta = -1;
        deadDelta = 1;
    }

    E_DIRECTION forwardDirection = E_DOWN;
    E_DIRECTION turnDirection = static_cast<E_DIRECTION>((forwardDirection + turnDelta) & 0x3);
    E_DIRECTION deadDirection = static_cast<E_DIRECTION>((forwardDirection + deadDelta) & 0x3);

    auto refreshDirection = [&](E_DIRECTION d)
    {
        forwardDirection = d;
        turnDirection = static_cast<E_DIRECTION>((forwardDirection + turnDelta) & 0x3);
        deadDirection = static_cast<E_DIRECTION>((forwardDirection + deadDelta) & 0x3);
    };

    SolutionList result;
    int currentNode = 0;
    const int end = adjList.m_iWidth * adjList.m_iHeight - 1;
    while (currentNode != end)
    {
        int forwardNode = currentNode + delta[forwardDirection];
        int turnNode = currentNode + delta[turnDirection];

        if (find(adjList.m_vVertexes[currentNode].begin(), adjList.m_vVertexes[currentNode].end(), turnNode) != adjList.m_vVertexes[currentNode].end())
        {
            result.m_vTrace.push_back(pair<int, int>(currentNode, turnNode));
            refreshDirection(turnDirection);
            currentNode = turnNode;
        }
        else if (find(adjList.m_vVertexes[currentNode].begin(), adjList.m_vVertexes[currentNode].end(), forwardNode) != adjList.m_vVertexes[currentNode].end())
        {
            result.m_vTrace.push_back(pair<int, int>(currentNode, forwardNode));
            currentNode = forwardNode;
        }
        else
        {
            refreshDirection(deadDirection);
        }
    }
    result.setupUseTrace();
    return result;
}
