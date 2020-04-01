#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <vector>
#include <set>

struct AdjacencyList
{
    AdjacencyList(int row = -1, int column = -1);

    // init list with all adjacent point linked
    void linkAllNodes();
    // init list with all adjacent point unlinked
    void unlinkAllNodes();

    void generateAllLink();

    void connect(int i, int j);
    void unconnect(int i, int j);

    // check the position of point
    bool isLeftTop(int index);
    bool isRightTop(int index);
    bool isLeftBottom(int index);
    bool isRightBottom(int index);
    bool isLeftEdge(int index);
    bool isRightEdge(int index);
    bool isTopEdge(int index);
    bool isBottomEdge(int index);

    // return the node num having 1/2/3/4 neighbors
    static std::vector<int> neighborStat(AdjacencyList &list);

    int m_row;
    int m_column;

    std::vector<std::vector<int>> m_nodes;
    std::vector<std::vector<int>> m_nodesAllLinked;
};

#endif // ADJACENCYLIST_H
