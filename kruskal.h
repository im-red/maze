#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "adjacencylist.h"

class Kruskal
{
public:
    Kruskal(int width, int height);
    AdjacencyList generate();

private:
    int m_iWidth;
    int m_iHeight;
};

#endif // KRUSKAL_H
