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

#include "recursivedivision.h"
#include "util.h"

#include <random>
#include <ctime>
#include <vector>
#include <utility>

using namespace std;

RecursiveDivision::RecursiveDivision(int row, int column)
    : m_row(row)
    , m_column(column)
{

}

AdjacencyList RecursiveDivision::generate()
{
    StopWatch sw(__FUNCTION__);

    AdjacencyList result(m_row, m_column);
    result.connectAllSurround();

    divide(result, 0, 0, m_column - 1, m_row - 1);
    return result;
}

void RecursiveDivision::divide(AdjacencyList &list, int left, int top, int right, int bottom)
{
    // the x range of input is [left, right]
    // the y range of input is [top, bottom]

    if ((right - left < 1) || (bottom - top < 1))
    {
        return;
    }

    static default_random_engine e(time(nullptr));
    int x = e() % (right - left) + left;
    int y = e() % (bottom - top) + top;

    vector<pair<int, int>> toDisconnect;

    for (int i = left; i <= right; i++)
    {
        int p = y * m_column + i;
        int q = (y + 1) * m_column + i;
        toDisconnect.emplace_back(p, q);
    }

    for (int i = top; i <= bottom; i++)
    {
        int p = i * m_column + x;
        int q = i * m_column + x + 1;
        toDisconnect.emplace_back(p, q);
    }

    // the position of no gap wall relative to (x, y), 0:top 1:bottom 2:left 3:right
    int position = e() % 4;

    int gapPos[4] = {0};

    // get the gap position
    gapPos[0] = e() % (y - top + 1) + top;
    gapPos[1] = e() % (bottom - y) + y + 1;
    gapPos[2] = e() % (x - left + 1) + left;
    gapPos[3] = e() % (right - x) + x + 1;

    for (int i = 0; i <= 3; i++)
    {
        if (position != i)
        {
            int p = 0;
            int q = 0;
            if (i <= 1) // the gap is in top or bottom
            {
                p = gapPos[i] * m_column + x;
                q = gapPos[i] * m_column + x + 1;
            }
            else // the gap is in left or right
            {
                p = y * m_column + gapPos[i];
                q = (y + 1) * m_column + gapPos[i];
            }
            pair<int, int> pair(p, q);
            toDisconnect.erase(find(toDisconnect.begin(), toDisconnect.end(), pair));
        }
    }

    for (auto &pair : toDisconnect)
    {
        list.disconnect(pair.first, pair.second);
    }

    divide(list, left, top, x, y);
    divide(list, x + 1, top, right, y);
    divide(list, left, y + 1, x, bottom);
    divide(list, x + 1, y + 1, right, bottom);
}
