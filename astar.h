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

#include "adjacencylist.h"
#include "solutionlist.h"

struct AStarNode
{
    AStarNode(int id = -1) : m_iId(id), m_iF(0), m_iG(0), m_iH(0) {}
    int m_iId;
    int m_iF;
    int m_iG;
    int m_iH;
};

class AStar
{
public:
    enum HFunc
    {
        Manhattan,
        Euclidian,
        Zero
    };
    explicit AStar(HFunc func);

    SolutionList solve(const AdjacencyList &adjList);

private:
    static int manhattanDistance(int p, int q, int width);
    static int euclidianDistance(int p, int q, int width);
    static int zeroDistance(int p, int q, int width);
    HFunc m_hFuncType;
};
