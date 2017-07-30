#include "mazewidget.h"

#include <QPainter>
#include <QPen>
#include <iostream>
#include <QDebug>
#include <QPixmap>
#include <QFileDialog>

MazeWidget::MazeWidget(QWidget *parent)
    : QWidget(parent),
      m_data(0, 0)
{

}

void MazeWidget::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, "", "", tr("Image Files (*.png)"));
    if (fileName.isEmpty())
    {
        return;
    }
    //QPixmap pixmap(this->size());
    QPixmap pixmap(this->width() + 2 * m_iPicSpacing, this->height() + 2 * m_iPicSpacing);
    this->render(&pixmap, QPoint(m_iPicSpacing, m_iPicSpacing));
    pixmap.save(fileName, "png");
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

    int row_spacing = (dynamic_cast<QWidget *>(this->parent())->height() - 1) / row;
    int column_spacing = (dynamic_cast<QWidget *>(this->parent())->width() - 1) / column;
    m_iSpacing = row_spacing > column_spacing ? column_spacing : row_spacing;
    m_iSpacing = m_iSpacing > m_iMinSpacing ? m_iSpacing : m_iMinSpacing;
    this->setMinimumSize(column * m_iSpacing + 1, row * m_iSpacing + 1);

    QPainter painter;
    painter.begin(&m_picture);
    painter.fillRect(0, 0, m_iSpacing * column, m_iSpacing * row, Qt::white);

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
                    painter.drawLine(j * m_iSpacing, (i / 2) * m_iSpacing, (j + 1) * m_iSpacing, (i / 2) * m_iSpacing);
                }
            }
        }
        else
        {
            for (int j = 0; j < column + 1; j++)
            {
                if (a.at(j))
                {
                    painter.drawLine(j * m_iSpacing, (i / 2) * m_iSpacing, j * m_iSpacing, (i / 2 + 1) * m_iSpacing);
                }
            }
        }
    }

    painter.fillRect(1, 1, m_iSpacing - 3, m_iSpacing - 3, Qt::red);
    painter.fillRect(m_iSpacing * (column - 1) + 3, m_iSpacing * (row - 1) + 3, m_iSpacing - 3, m_iSpacing - 3, Qt::green);
    painter.end();

    QPainter screenPainter(this);
    m_picture.play(&screenPainter);
}
