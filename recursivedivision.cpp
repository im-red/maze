#include "recursivedivision.h"

#include <random>
#include <ctime>

using namespace std;

RecursiveDivision::RecursiveDivision(int row, int column)
    : m_row(row)
    , m_column(column)
{

}

AdjacencyList RecursiveDivision::generate()
{
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

    for (int i = left; i <= right; i++)
    {
        int p = y * m_column + i;
        int q = (y + 1) * m_column + i;
        list.disconnect(p, q);
    }

    for (int i = top; i <= bottom; i++)
    {
        int p = i * m_column + x;
        int q = i * m_column + x + 1;
        list.disconnect(p, q);
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
            if (i <= 1) // the gap is in top of bottom
            {
                p = gapPos[i] * m_column + x;
                q = gapPos[i] * m_column + x + 1;
            }
            else // the gap is in left or right
            {
                p = y * m_column + gapPos[i];
                q = (y + 1) * m_column + gapPos[i];
            }
            list.connect(p, q);
        }
    }

    divide(list, left, top, x, y);
    divide(list, x + 1, top, right, y);
    divide(list, left, y + 1, x, bottom);
    divide(list, x + 1, y + 1, right, bottom);
}
