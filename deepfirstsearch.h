#ifndef DEEPFIRSTSEARCH_H
#define DEEPFIRSTSEARCH_H

#include "adjacencylist.h"

class DeepFirstSearch
{
public:
    enum E_COLOR
    {
        E_WHITE,
        E_GRAY,
        E_BLACK
    };
    DeepFirstSearch(int width, int height);
    AdjacencyList generate();

private:
    int m_iWidth;
    int m_iHeight;
};

#endif // DEEPFIRSTSEARCH_H
