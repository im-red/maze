#include "mazedata.h"

MazeData::MazeData(int row, int column)
    : m_iRow(row),
      m_iColumn(column)

{
    if (0 == row || 0 == column)
    {
        return;
    }
    /*
     *  - - -
     * | | | |
     *  - - -
     * | | | |
     *  - - -
     * | | | |
     *  - - -
     * the data represents the edge
     */
    for (int i = 0; i < 2 * row + 1; i++)
    {
        m_vData.push_back(QBitArray(i % 2 == 0 ? column : column + 1, true));
    }
}
