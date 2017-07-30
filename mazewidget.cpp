#include "mazewidget.h"

#include <QPainter>
#include <QPen>
#include <iostream>
#include <QDebug>

MazeWidget::MazeWidget(QWidget *parent)
    : QWidget(parent),
      m_data(0, 0)
{

}

void MazeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    int row = m_data.m_iRow;
    int column = m_data.m_iColumn;

    if (0 == row || 0 == column)
    {
        return;
    }

    int row_spacing = (dynamic_cast<QWidget *>(this->parent())->height() - 3) / row;
    int column_spacing = (dynamic_cast<QWidget *>(this->parent())->width() - 3) / column;
    m_spacing = row_spacing > column_spacing ? column_spacing : row_spacing;
    m_spacing = m_spacing > m_minSpacing ? m_spacing : m_minSpacing;
    this->setMinimumSize(column * m_spacing + 3, row * m_spacing + 3);

    QPainter painter(this);
    painter.fillRect(0, 0, m_spacing * column, m_spacing * row, Qt::white);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for (int i = 0; i < 2 * row + 1; i++)
    {
        QBitArray a = m_data.m_vData.at(i);
        if (i % 2 == 0)
        {
            for (int j = 0; j < column; j++)
            {
                if (a.at(j))
                {
                    painter.drawLine(j * m_spacing, (i / 2) * m_spacing, (j + 1) * m_spacing, (i / 2) * m_spacing);
                }
            }
        }
        else
        {
            for (int j = 0; j < column + 1; j++)
            {
                if (a.at(j))
                {
                    painter.drawLine(j * m_spacing, (i / 2) * m_spacing, j * m_spacing, (i / 2 + 1) * m_spacing);
                }
            }
        }
    }

    painter.fillRect(1, 1, m_spacing - 3, m_spacing - 3, Qt::red);
    painter.fillRect(m_spacing * (column - 1) + 3, m_spacing * (row - 1) + 3, m_spacing - 3, m_spacing - 3, Qt::green);
}
