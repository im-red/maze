#ifndef DEEPFIRSTSEARCH_H
#define DEEPFIRSTSEARCH_H

#include "adjacencylist.h"

class DeepFirstSearch
{
public:
    DeepFirstSearch(int row, int column);
    AdjacencyList generate();

private:
    int m_row;
    int m_column;
};

#endif // DEEPFIRSTSEARCH_H
