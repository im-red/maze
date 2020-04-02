#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <vector>
#include <set>

class AdjacencyList
{
public:
    AdjacencyList(int row = 0, int column = 0);

    // init list with all surround point connected
    void connectAllSurround();

    void connect(int i, int j);
    void disconnect(int i, int j);

    int row() const { return m_row; }
    int column() const { return m_column; }

    int valid() const { return m_row > 1 && m_column > 1; }
    int nodeCount() const { return m_row * m_column; }
    bool validIndex(int i) const { return i >= 0 && i < nodeCount(); }

    const std::vector<int> &neighbor(int i) const;
    std::vector<int> &neighbor(int i);

    const std::vector<int> &surround(int i) const;
    std::vector<int> &surround(int i);

    // return the node num having 1/2/3/4 neighbors
    std::vector<int> neighborStat() const;

private:
    void fillSurround();

    // check the position of point
    bool isLeftTop(int index);
    bool isRightTop(int index);
    bool isLeftBottom(int index);
    bool isRightBottom(int index);
    bool isLeftEdge(int index);
    bool isRightEdge(int index);
    bool isTopEdge(int index);
    bool isBottomEdge(int index);

private:
    int m_row;
    int m_column;

    std::vector<std::vector<int>> m_index2neighbor;
    std::vector<std::vector<int>> m_index2surround;
};

#endif // ADJACENCYLIST_H
