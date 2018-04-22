#ifndef RECURSIVEDIVISION_H
#define RECURSIVEDIVISION_H

#include "adjacencylist.h"

class RecursiveDivision
{
public:
    RecursiveDivision(int width, int height);
    AdjacencyList generate();

private:
    void divide(AdjacencyList &list, int left, int top, int right, int bottom);
    int m_iWidth;
    int m_iHeight;
};

#endif // RECURSIVEDIVISION_H
