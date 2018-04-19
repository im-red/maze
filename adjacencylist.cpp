#include "adjacencylist.h"

#include <cassert>

using namespace std;

AdjacencyList::AdjacencyList()
    : m_iWidth(-1)
    , m_iHeight(-1)
{

}

void AdjacencyList::linkAllVertexes()
{
    assert(m_iWidth > 1 && m_iHeight > 1);

    m_vVertexes = vector<set<int>>(m_iWidth * m_iHeight);

    generateAllLink();
    m_vVertexes = m_vVertexesAllLinked;
}

void AdjacencyList::unlinkAllVertexes()
{
    assert(m_iWidth > 1 && m_iHeight > 1);

    m_vVertexes = vector<set<int>>(m_iWidth * m_iHeight);

    generateAllLink();
}

void AdjacencyList::generateAllLink()
{
    m_vVertexesAllLinked = vector<set<int>>(m_iWidth * m_iHeight);

    int size = m_iWidth * m_iHeight;
    for (int i = 0; i < size; i++)
    {
        if (!(isLeftTop(i) || isRightTop(i) || isTopEdge(i)))
        {
            m_vVertexesAllLinked[i].insert(i - m_iWidth);
        }
        if (!(isLeftBottom(i) || isRightBottom(i) || isBottomEdge(i)))
        {
            m_vVertexesAllLinked[i].insert(i + m_iWidth);
        }
        if (!(isLeftTop(i) || isLeftBottom(i) || isLeftEdge(i)))
        {
            m_vVertexesAllLinked[i].insert(i - 1);
        }
        if (!(isRightTop(i) || isRightBottom(i) || isRightEdge(i)))
        {
            m_vVertexesAllLinked[i].insert(i + 1);
        }
    }
}

void AdjacencyList::connect(int i, int j)
{
    assert(i >= 0 && i < m_iWidth * m_iHeight && j >= 0 && j < m_iWidth * m_iHeight);
    m_vVertexes[i].insert(j);
    m_vVertexes[j].insert(i);
}

bool AdjacencyList::isLeftTop(int index)
{
    assert(index >= 0 && index < m_iWidth * m_iHeight);
    return index == 0;
}

bool AdjacencyList::isRightTop(int index)
{
    assert(index >= 0 && index < m_iWidth * m_iHeight);
    return index == (m_iWidth - 1);
}

bool AdjacencyList::isLeftBottom(int index)
{
    assert(index >= 0 && index < m_iWidth * m_iHeight);
    return index == m_iWidth * (m_iHeight - 1);
}

bool AdjacencyList::isRightBottom(int index)
{
    assert(index >= 0 && index < m_iWidth * m_iHeight);
    return index == (m_iHeight * m_iWidth - 1);
}

bool AdjacencyList::isLeftEdge(int index)
{
    assert(index >= 0 && index < m_iWidth * m_iHeight);
    if (isLeftTop(index) || isLeftBottom(index))
    {
        return false;
    }
    return index % m_iWidth == 0;
}

bool AdjacencyList::isRightEdge(int index)
{
    assert(index >= 0 && index < m_iWidth * m_iHeight);
    if (isRightTop(index) || isRightBottom(index))
    {
        return false;
    }
    return index % m_iWidth == (m_iWidth - 1);
}

bool AdjacencyList::isTopEdge(int index)
{
    assert(index >= 0 && index < m_iWidth * m_iHeight);
    if (isLeftTop(index) || isRightTop(index))
    {
        return false;
    }
    return index > 0 && index < (m_iWidth - 1);
}

bool AdjacencyList::isBottomEdge(int index)
{
    assert(index >= 0 && index < m_iWidth * m_iHeight);
    if (isLeftBottom(index) || isRightBottom(index))
    {
        return false;
    }
    return index > (m_iWidth * (m_iHeight - 1)) && index < (m_iHeight * m_iWidth - 1);
}
