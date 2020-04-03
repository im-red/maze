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

#include "unionfind.h"

using namespace std;

UnionFind::UnionFind(int n)
    : m_nodeCount(n)
    , m_connectionCount(0)
    , m_index2parent(n)
    , m_index2treeSize(n)
{
    for (int i = 0; i < n; i++)
    {
        m_index2parent[i] = i;
        m_index2treeSize[i] = 1;
    }
}

UnionFind::~UnionFind()
{

}

void UnionFind::connect(int p, int q)
{
    int i = root(p);
    int j = root(q);

    if (i == j)
    {
        return;
    }
    if (m_index2treeSize[i] < m_index2treeSize[j])
    {
        m_index2parent[i] = j;
        m_index2treeSize[j] += m_index2treeSize[i];
    }
    else
    {
        m_index2parent[j] = i;
        m_index2treeSize[i] += m_index2treeSize[j];
    }
    m_connectionCount++;
}

int UnionFind::root(int p)
{
    while (p != m_index2parent[p])
    {
        p = m_index2parent[p];
    }
    return p;
}
