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

#include "deepfirstsearch.h"

#include <vector>
#include <algorithm>

using namespace std;

DeepFirstSearch::DeepFirstSearch(int row, int column)
    : m_row(row)
    , m_column(column)
{

}

AdjacencyList DeepFirstSearch::generate()
{
    enum Color
    {
        White,
        Gray,
        Black
    };

    AdjacencyList result(m_row, m_column);

    vector<int> color(static_cast<size_t>(m_row * m_column), White);
    vector<int> current;
    current.reserve(static_cast<size_t>(m_row * m_column));

    color[0] = Gray;
    current.push_back(0);

    while (current.size())
    {
        int last = current.back();
        random_shuffle(result.surround(last).begin(), result.surround(last).end());

        auto iter = result.surround(last).cbegin();

        for (; iter != result.surround(last).cend(); iter++)
        {
            if (color[static_cast<size_t>(*iter)] == White)
            {
                color[static_cast<size_t>(*iter)] = Gray;
                result.connect(last, *iter);
                current.push_back(*iter);
                break;
            }
        }

        // all adjacent points are found
        if (iter == result.surround(last).cend())
        {
            current.pop_back();
            color[static_cast<size_t>(last)] = Black;
        }
    }

    return result;
}
