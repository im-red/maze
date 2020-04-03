/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2020 Jia Lihong
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

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
    delta[Up] = -adjList.column();
    delta[Down] = adjList.column();
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
    const int end = adjList.nodeCount() - 1;
    while (currentNode != end)
    {
        int forwardNode = currentNode + delta[forwardDirection];
        int turnNode = currentNode + delta[turnDirection];

        if (find(adjList.neighbor(currentNode).begin(), adjList.neighbor(currentNode).end(), turnNode) != adjList.neighbor(currentNode).end())
        {
            result.m_trace.push_back(pair<int, int>(currentNode, turnNode));
            refreshDirection(turnDirection);
            currentNode = turnNode;
        }
        else if (find(adjList.neighbor(currentNode).begin(), adjList.neighbor(currentNode).end(), forwardNode) != adjList.neighbor(currentNode).end())
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
