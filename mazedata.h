#ifndef MAZEDATA_H
#define MAZEDATA_H

#include <QVector>
#include <QBitArray>

class MazeData
{
public:
    MazeData(int row, int column);

public:
    int m_iRow;
    int m_iColumn;
    QVector<QBitArray> m_vData;
};

#endif // MAZEDATA_H
