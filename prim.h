#ifndef PRIM_H
#define PRIM_H

#include "adjacencylist.h"

class Prim
{
public:
    Prim(int width, int height);
    AdjacencyList generate();

private:
    int m_iWidth;
    int m_iHeight;
};

#endif // PRIM_H
