#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <vector>
#include <set>

struct AdjacencyList
{
    AdjacencyList();

    // 以连接相邻顶点重置邻接链表
    void linkAllVertexes();
    // 以不连接相邻顶点重置邻接链表
    void unlinkAllVertexes();

    void generateAllLink();

    void connect(int i, int j);

    // 判断顶点的位置
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
