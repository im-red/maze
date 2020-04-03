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

#include "kruskal.h"
#include "unionfind.h"

#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

Kruskal::Kruskal(int row, int column)
    : m_row(row)
    , m_column(column)
{

}

AdjacencyList Kruskal::generate()
{
    AdjacencyList result(m_row, m_column);

    UnionFind uf(m_row * m_column);

    vector<pair<int, int>> edges;
    for (int i = 0; i < m_row * m_column; i++)
    {
        for (auto iter : result.surround(i))
        {
            // avoid duplicate edge
            if (i > iter)
            {
                edges.push_back(pair<int, int>(i, iter));
            }
        }
    }
    random_shuffle(edges.begin(), edges.end());
    for (auto iter : edges)
    {
        if(!uf.connected(iter.first, iter.second))
        {
            uf.connect(iter.first, iter.second);
            result.connect(iter.first, iter.second);
        }
    }
    return result;
}
