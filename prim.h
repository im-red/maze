#ifndef PRIM_H
#define PRIM_H

#include "adjacencylist.h"

class Prim
{
public:
    Prim(int row, int column);
    AdjacencyList generate();

private:
    int m_row;
    int m_column;
};

#endif // PRIM_H
