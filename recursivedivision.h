#ifndef RECURSIVEDIVISION_H
#define RECURSIVEDIVISION_H

#include "adjacencylist.h"

class RecursiveDivision
{
public:
    RecursiveDivision(int row, int column);
    AdjacencyList generate();

private:
    void divide(AdjacencyList &list, int left, int top, int right, int bottom);

private:
    int m_row;
    int m_column;
};

#endif // RECURSIVEDIVISION_H
