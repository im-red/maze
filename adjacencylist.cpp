#include "adjacencylist.h"

#include <cassert>

using namespace std;

AdjacencyList::AdjacencyList(int row, int column)
    : m_row(row)
    , m_column(column)
    , m_index2neighbor(m_row * m_column)
    , m_index2surround(m_row * m_column)

{
    fillSurround();
}

void AdjacencyList::connectAllSurround()
{
    assert(valid());
    m_index2neighbor = m_index2surround;
}

void AdjacencyList::fillSurround()
{
    int size = nodeCount();
    for (int i = 0; i < size; i++)
    {
        if (!(isLeftTop(i) || isRightTop(i) || isTopEdge(i)))
        {
            m_index2surround[i].push_back(i - m_column);
        }
        if (!(isLeftBottom(i) || isRightBottom(i) || isBottomEdge(i)))
        {
            m_index2surround[i].push_back(i + m_column);
        }
        if (!(isLeftTop(i) || isLeftBottom(i) || isLeftEdge(i)))
        {
            m_index2surround[i].push_back(i - 1);
        }
        if (!(isRightTop(i) || isRightBottom(i) || isRightEdge(i)))
        {
            m_index2surround[i].push_back(i + 1);
        }
    }
}

void AdjacencyList::connect(int i, int j)
{
    assert(valid() && validIndex(i) && validIndex(j));
    m_index2neighbor[i].push_back(j);
    m_index2neighbor[j].push_back(i);
}

void AdjacencyList::disconnect(int i, int j)
{
    assert(valid() && validIndex(i) && validIndex(j));
    m_index2neighbor[i].erase(find(m_index2neighbor[i].begin(), m_index2neighbor[i].end(), j));
    m_index2neighbor[j].erase(find(m_index2neighbor[j].begin(), m_index2neighbor[j].end(), i));
}

bool AdjacencyList::isLeftTop(int index)
{
    assert(validIndex(index));
    return index == 0;
}

bool AdjacencyList::isRightTop(int index)
{
    assert(validIndex(index));
    return index == (m_column - 1);
}

bool AdjacencyList::isLeftBottom(int index)
{
    assert(validIndex(index));
    return index == (m_row - 1) * m_column;
}

bool AdjacencyList::isRightBottom(int index)
{
    assert(validIndex(index));
    return index == (m_row * m_column - 1);
}

bool AdjacencyList::isLeftEdge(int index)
{
    assert(validIndex(index));
    if (isLeftTop(index) || isLeftBottom(index))
    {
        return false;
    }
    return index % m_column == 0;
}

bool AdjacencyList::isRightEdge(int index)
{
    assert(validIndex(index));
    if (isRightTop(index) || isRightBottom(index))
    {
        return false;
    }
    return index % m_column == (m_column - 1);
}

bool AdjacencyList::isTopEdge(int index)
{
    assert(validIndex(index));
    if (isLeftTop(index) || isRightTop(index))
    {
        return false;
    }
    return index > 0 && index < (m_column - 1);
}

bool AdjacencyList::isBottomEdge(int index)
{
    assert(validIndex(index));
    if (isLeftBottom(index) || isRightBottom(index))
    {
        return false;
    }
    return index > ((m_row - 1) * m_column) && index < (m_row * m_column - 1);
}

vector<int> AdjacencyList::neighborStat() const
{
    assert(valid());
    vector<int> result(4, 0);
    for (int i = 0; i < nodeCount(); i++)
    {
        assert(neighbor(i).size() >= 1 && neighbor(i).size() <= 4);
        result[neighbor(i).size() - 1]++;
    }
    return result;
}

const std::vector<int> &AdjacencyList::neighbor(int i) const
{
    assert(validIndex(i));
    return m_index2neighbor[i];
}

std::vector<int> &AdjacencyList::neighbor(int i)
{
    assert(validIndex(i));
    return m_index2neighbor[i];
}

const std::vector<int> &AdjacencyList::surround(int i) const
{
    assert(validIndex(i));
    return m_index2surround[i];
}

std::vector<int> &AdjacencyList::surround(int i)
{
    assert(validIndex(i));
    return m_index2surround[i];
}
