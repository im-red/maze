/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2020 Jia Lihong
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

#pragma once

#include <vector>
#include <set>

class AdjacencyList
{
public:
    enum GenerationActionType
    {
        BuildWall,
        BreakWall
    };

public:
    AdjacencyList(int row = 0, int column = 0);

    // init list with all surround point connected
    void connectAllSurround();

    void connect(int i, int j);
    void disconnect(int i, int j);

    int row() const { return m_row; }
    int column() const { return m_column; }

    int valid() const { return m_row > 1 && m_column > 1; }
    int nodeCount() const { return m_row * m_column; }
    bool validIndex(int i) const { return i >= 0 && i < nodeCount(); }

    const std::vector<int> &neighbor(int i) const;
    std::vector<int> &neighbor(int i);

    const std::vector<int> &surround(int i) const;
    std::vector<int> &surround(int i);

    // return the node num having 1/2/3/4 neighbors
    std::vector<int> neighborStat() const;

    GenerationActionType generationActionType() const { return m_gaType; }
    int generationActionCount() const { return static_cast<int>(m_gaActions.size()); }
    std::vector<std::pair<int, int>> generationActions() const { return m_gaActions; }

private:
    void fillSurround();

    // check the position of point
    bool isLeftTop(int index);
    bool isRightTop(int index);
    bool isLeftBottom(int index);
    bool isRightBottom(int index);
    bool isLeftEdge(int index);
    bool isRightEdge(int index);
    bool isTopEdge(int index);
    bool isBottomEdge(int index);

private:
    int m_row;
    int m_column;

    std::vector<std::vector<int>> m_index2neighbor;
    std::vector<std::vector<int>> m_index2surround;

    GenerationActionType m_gaType;
    std::vector<std::pair<int, int>> m_gaActions;
};
