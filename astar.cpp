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

#include "astar.h"
#include "mutable_priority_queue.h"
#include "util.h"

#include <queue>
#include <set>
#include <vector>
#include <functional>
#include <cmath>
#include <cassert>
#include <memory>
#include <cstdio>

using namespace std;

AStar::AStar(HFunc func)
    : m_hFuncType(func)
{

}

SolutionList AStar::solve(const AdjacencyList &adjList)
{
    const int column = adjList.column();
    const int row = adjList.row();

    function<int(int, int, int)> hFunc;
    switch (m_hFuncType)
    {
    case Manhattan:
        hFunc = manhattanDistance;
        break;
    case Euclidian:
        hFunc = euclidianDistance;
        break;
    case Zero:
        hFunc = zeroDistance;
        break;
    }

    MutablePriorityQueue<AStarNode> openQueue([](const AStarNode &p, const AStarNode &q) { return p.m_iF < q.m_iF; });
    set<int> closeSet;
    set<int> openSet;

    vector<int> parent(static_cast<size_t>(row * column), -1);
    vector<MutablePriorityQueue<AStarNode>::handle_type> handleVector(static_cast<size_t>(row * column));

    const int endIndex = row * column - 1;
    const int beginIndex = 0;
    AStarNode start(beginIndex);
    handleVector[static_cast<size_t>(start.m_iId)] = openQueue.push(start);
    openSet.insert(start.m_iId);

    SolutionList solution;

    while (!openQueue.empty())
    {
        // pop the least F value node to process
        AStarNode currentNode = openQueue.pop();
        openSet.erase(openSet.find(currentNode.m_iId));
        closeSet.insert(currentNode.m_iId);
        for (int neighbor : adjList.neighbor(currentNode.m_iId))
        {
            solution.m_accessed.insert(makeOrderedPair(currentNode.m_iId, neighbor));
            // if we found endIndex, it's end
            if (neighbor == endIndex)
            {
                parent[static_cast<size_t>(neighbor)] = currentNode.m_iId;
                // collect the path from endIndex to beginIndex
                for (int i = endIndex; i != beginIndex; i = parent[static_cast<size_t>(i)])
                {
                    solution.m_solution.push_back(pair<int, int>(i, parent[static_cast<size_t>(i)]));
                }
                return solution;
            }

            if (closeSet.count(neighbor) != 0) // the neighbor is already in close set
            {
                // skip the node already close
                continue;
            }
            else if (openSet.count(neighbor) == 0) // the neighbor is not in open set
            {
                // insert the node to open queue and open set
                AStarNode toInsert(neighbor);
                toInsert.m_iG = currentNode.m_iG + 1;
                toInsert.m_iH = hFunc(neighbor, endIndex, column);
                toInsert.m_iF = toInsert.m_iG + toInsert.m_iH;
                handleVector[static_cast<size_t>(neighbor)] = openQueue.push(toInsert);
                openSet.insert(neighbor);

                // set the parent
                parent[static_cast<size_t>(neighbor)] = currentNode.m_iId;
            }
            else // the neighbor is already in open set
            {
                AStarNode neighborNode = openQueue.value(handleVector[static_cast<size_t>(neighbor)]);
                int newG = currentNode.m_iG + 1;
                // if neighborNode has a better parent(a less G value), reparent and update it
                if (newG < neighborNode.m_iG)
                {
                    neighborNode.m_iG = newG;
                    neighborNode.m_iF = neighborNode.m_iH + newG;
                    openQueue.update(handleVector[static_cast<size_t>(neighbor)], neighborNode);
                    parent[static_cast<size_t>(neighbor)] = currentNode.m_iId;
                }
            }
        }
    }

    // no solution
    return SolutionList();
}

int AStar::manhattanDistance(int p, int q, int width)
{
    int x1 = p % width;
    int y1 = p / width;
    int x2 = q % width;
    int y2 = q / width;

    return abs(x1 - x2) + abs(y1 - y2);
}

int AStar::euclidianDistance(int p, int q, int width)
{
    int x1 = p % width;
    int y1 = p / width;
    int x2 = q % width;
    int y2 = q / width;

    return static_cast<int>(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

int AStar::zeroDistance(int p, int q, int width)
{
    (void)p;
    (void)q;
    (void)width;
    return 0;
}
