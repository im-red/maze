#include "adjacencylist.h"

#include <cassert>

using namespace std;

AdjacencyList::AdjacencyList(int row, int column)
    : m_row(row)
    , m_column(column)
{

}

void AdjacencyList::linkAllNodes()
{
    assert(m_row > 1 && m_column > 1);

    m_nodes = vector<vector<int>>(m_row * m_column);

    generateAllLink();
    m_nodes = m_nodesAllLinked;
}

void AdjacencyList::unlinkAllNodes()
{
    assert(m_row > 1 && m_column > 1);

    m_nodes = vector<vector<int>>(m_row * m_column);

    generateAllLink();
}

void AdjacencyList::generateAllLink()
{
    m_nodesAllLinked = vector<vector<int>>(m_row * m_column);

    int size = m_row * m_column;
    for (int i = 0; i < size; i++)
    {
        if (!(isLeftTop(i) || isRightTop(i) || isTopEdge(i)))
        {
            m_nodesAllLinked[i].push_back(i - m_column);
        }
        if (!(isLeftBottom(i) || isRightBottom(i) || isBottomEdge(i)))
        {
            m_nodesAllLinked[i].push_back(i + m_column);
        }
        if (!(isLeftTop(i) || isLeftBottom(i) || isLeftEdge(i)))
        {
            m_nodesAllLinked[i].push_back(i - 1);
        }
        if (!(isRightTop(i) || isRightBottom(i) || isRightEdge(i)))
        {
            m_nodesAllLinked[i].push_back(i + 1);
        }
    }
}

void AdjacencyList::connect(int i, int j)
{
    assert(i >= 0 && i < m_row * m_column && j >= 0 && j < m_row * m_column);
    m_nodes[i].push_back(j);
    m_nodes[j].push_back(i);
}

void AdjacencyList::unconnect(int i, int j)
{
    assert(i >= 0 && i < m_row * m_column && j >= 0 && j < m_row * m_column);
    m_nodes[i].erase(find(m_nodes[i].begin(), m_nodes[i].end(), j));
    m_nodes[j].erase(find(m_nodes[j].begin(), m_nodes[j].end(), i));
}

bool AdjacencyList::isLeftTop(int index)
{
    assert(index >= 0 && index < m_row * m_column);
    return index == 0;
}

bool AdjacencyList::isRightTop(int index)
{
    assert(index >= 0 && index < m_row * m_column);
    return index == (m_column - 1);
}

bool AdjacencyList::isLeftBottom(int index)
{
    assert(index >= 0 && index < m_row * m_column);
    return index == (m_row - 1) * m_column;
}

bool AdjacencyList::isRightBottom(int index)
{
    assert(index >= 0 && index < m_row * m_column);
    return index == (m_row * m_column - 1);
}

bool AdjacencyList::isLeftEdge(int index)
{
    assert(index >= 0 && index < m_row * m_column);
    if (isLeftTop(index) || isLeftBottom(index))
    {
        return false;
    }
    return index % m_column == 0;
}

bool AdjacencyList::isRightEdge(int index)
{
    assert(index >= 0 && index < m_row * m_column);
    if (isRightTop(index) || isRightBottom(index))
    {
        return false;
    }
    return index % m_column == (m_column - 1);
}

bool AdjacencyList::isTopEdge(int index)
{
    assert(index >= 0 && index < m_row * m_column);
    if (isLeftTop(index) || isRightTop(index))
    {
        return false;
    }
    return index > 0 && index < (m_column - 1);
}

bool AdjacencyList::isBottomEdge(int index)
{
    assert(index >= 0 && index < m_row * m_column);
    if (isLeftBottom(index) || isRightBottom(index))
    {
        return false;
    }
    return index > ((m_row - 1) * m_column) && index < (m_row * m_column - 1);
}

vector<int> AdjacencyList::neighborStat(AdjacencyList &list)
{
    assert(list.m_row > 0 && list.m_column > 0);
    vector<int> result(4, 0);
    for (int i = 0; i < list.m_row * list.m_column; i++)
    {
        assert(list.m_nodes[i].size() >= 1 && list.m_nodes[i].size() <= 4);
        result[list.m_nodes[i].size() - 1]++;
    }
    return result;
}
