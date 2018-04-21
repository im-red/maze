#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <vector>
#include <set>

struct AdjacencyList
{
    AdjacencyList(int width, int height);

    // init list with all adjacent point linked
    void linkAllVertexes();
    // init list with all adjacent point unlinked
    void unlinkAllVertexes();

    void generateAllLink();

    void connect(int i, int j);

    // check the position of point
    bool isLeftTop(int index);
    bool isRightTop(int index);
    bool isLeftBottom(int index);
    bool isRightBottom(int index);
    bool isLeftEdge(int index);
    bool isRightEdge(int index);
    bool isTopEdge(int index);
    bool isBottomEdge(int index);

    int m_iWidth;
    int m_iHeight;
    std::vector<std::vector<int>> m_vVertexes;
    std::vector<std::vector<int>> m_vVertexesAllLinked;
};

#endif // ADJACENCYLIST_H
