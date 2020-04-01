#include "astar.h"
#include "mutable_priority_queue.h"

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

SolutionList AStar::solve(AdjacencyList &adjList)
{
    const int column = adjList.m_column;
    const int row = adjList.m_row;

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
    default:
        assert(false);
        break;
    }

    MutablePriorityQueue<AStarNode> openQueue([](const AStarNode &p, const AStarNode &q) { return p.m_iF < q.m_iF; });
    set<int> closeSet;
    set<int> openSet;

    vector<int> parent(row * column, -1);
    vector<MutablePriorityQueue<AStarNode>::handle_type> handleVector(row * column);

    const int endIndex = row * column - 1;
    const int beginIndex = 0;
    AStarNode start(beginIndex);
    handleVector[start.m_iId] = openQueue.push(start);
    openSet.insert(start.m_iId);

    SolutionList solution;

    while (!openQueue.empty())
    {
        // pop the least F value node to process
        AStarNode currentNode = openQueue.pop();
        openSet.erase(openSet.find(currentNode.m_iId));
        closeSet.insert(currentNode.m_iId);
        for (int neighbor : adjList.m_nodes[currentNode.m_iId])
        {
            solution.m_accessed.insert(SolutionList::makePair(currentNode.m_iId, neighbor));
            // if we found endIndex, it's end
            if (neighbor == endIndex)
            {
                parent[neighbor] = currentNode.m_iId;
                // collect the path from endIndex to beginIndex
                for (int i = endIndex; i != beginIndex; i = parent[i])
                {
                    solution.m_solution.push_back(pair<int, int>(i, parent[i]));
                }
                return solution;
            }

            // the neighbor is already in close set
            if (closeSet.count(neighbor) != 0)
            {
                // skip the node already close
                continue;
            }
            // the neighbor is not in open set
            else if (openSet.count(neighbor) == 0)
            {
                // insert the node to open queue and open set
                AStarNode toInsert(neighbor);
                toInsert.m_iG = currentNode.m_iG + 1;
                toInsert.m_iH = hFunc(neighbor, endIndex, column);
                toInsert.m_iF = toInsert.m_iG + toInsert.m_iH;
                handleVector[neighbor] = openQueue.push(toInsert);
                openSet.insert(neighbor);

                // set the parent
                parent[neighbor] = currentNode.m_iId;
            }
            // the neighbor is already in open set
            else
            {
                AStarNode neighborNode = openQueue.value(handleVector[neighbor]);
                int newG = currentNode.m_iG + 1;
                // if neighborNode has a better parent(a less G value), reparent and update it
                if (newG < neighborNode.m_iG)
                {
                    neighborNode.m_iG = newG;
                    neighborNode.m_iF = neighborNode.m_iH + newG;
                    openQueue.update(handleVector[neighbor], neighborNode);
                    parent[neighbor] = currentNode.m_iId;
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
