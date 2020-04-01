#include "wallfollower.h"

#include <map>

using namespace std;

WallFollower::WallFollower(WallFollower::Hand hand)
    : m_hand(hand)
{

}

SolutionList WallFollower::solve(AdjacencyList &adjList)
{
    enum Direction
    {
        Down = 0, // 00
        Left = 1, // 01
        Up = 2,   // 10
        Right = 3 // 11
    };

    map<Direction, int> delta;
    delta[Up] = -adjList.m_column;
    delta[Down] = adjList.m_column;
    delta[Left] = -1;
    delta[Right] = 1;

    int deadDelta = 0;
    int turnDelta = 0;

    if (m_hand == RightHand)
    {
        turnDelta = 1;
        deadDelta = -1;
    }
    else
    {
        turnDelta = -1;
        deadDelta = 1;
    }

    Direction forwardDirection = Down;
    Direction turnDirection = static_cast<Direction>((forwardDirection + turnDelta) & 0x3);
    Direction deadDirection = static_cast<Direction>((forwardDirection + deadDelta) & 0x3);

    auto refreshDirection = [&](Direction d)
    {
        forwardDirection = d;
        turnDirection = static_cast<Direction>((forwardDirection + turnDelta) & 0x3);
        deadDirection = static_cast<Direction>((forwardDirection + deadDelta) & 0x3);
    };

    SolutionList result;
    int currentNode = 0;
    const int end = adjList.m_row * adjList.m_column - 1;
    while (currentNode != end)
    {
        int forwardNode = currentNode + delta[forwardDirection];
        int turnNode = currentNode + delta[turnDirection];

        if (find(adjList.m_nodes[currentNode].begin(), adjList.m_nodes[currentNode].end(), turnNode) != adjList.m_nodes[currentNode].end())
        {
            result.m_trace.push_back(pair<int, int>(currentNode, turnNode));
            refreshDirection(turnDirection);
            currentNode = turnNode;
        }
        else if (find(adjList.m_nodes[currentNode].begin(), adjList.m_nodes[currentNode].end(), forwardNode) != adjList.m_nodes[currentNode].end())
        {
            result.m_trace.push_back(pair<int, int>(currentNode, forwardNode));
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
