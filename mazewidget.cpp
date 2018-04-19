#include "mazewidget.h"

#include <QPainter>
#include <QPen>
#include <iostream>
#include <QDebug>
#include <QPixmap>
#include <QFileDialog>

#include <cassert>

using namespace std;

MazeWidget::MazeWidget(QWidget *parent)
    : QLabel(parent)
    , m_iShowWhat(E_WALL)
    , m_wallData(0, 0)
{

}

void MazeWidget::setWall(MazeData &data)
{
    m_wallData = data;

    int row = m_wallData.m_iRow;
    int column = m_wallData.m_iColumn;

    assert(row >= 2 && column >= 2);

    adjustSpacing(column, row);
    m_wall = QImage(column * m_iSpacing + 1, row * m_iSpacing + 1, QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&m_wall);
    painter.fillRect(0, 0, m_iSpacing * column, m_iSpacing * row, Qt::white);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for (int i = 0; i < 2 * row + 1; i++)
    {
        QBitArray a = m_wallData.m_vData.at(i);
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
    setPixmap(QPixmap::fromImage(m_wall));
}

void MazeWidget::setPath(AdjacencyList &list)
{
    m_pathData = list;

    int width = m_pathData.m_iWidth;
    int height = m_pathData.m_iHeight;

    assert(width >= 2 && height >= 2);

    adjustSpacing(width, height);
    QImage image = QImage(width * m_iSpacing + 1, height * m_iSpacing + 1, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_Plus);

    if (m_iShowWhat & E_PATH)
    {
        generatePath();
        painter.drawImage(0, 0, m_path);
    }
    painter.setCompositionMode(QPainter::CompositionMode_Plus);
    if (m_iShowWhat & E_WALL)
    {
        generateWall();
        painter.drawImage(0, 0, m_wall);
    }

    painter.end();
    setPixmap(QPixmap::fromImage(image));
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

void MazeWidget::adjustSpacing(int width, int height)
{
    int row_spacing = (dynamic_cast<QWidget *>(this->parent())->height() - 1) / height;
    int column_spacing = (dynamic_cast<QWidget *>(this->parent())->width() - 1) / width;
    m_iSpacing = row_spacing > column_spacing ? column_spacing : row_spacing;
    m_iSpacing = m_iSpacing > m_iMinSpacing ? m_iSpacing : m_iMinSpacing;
}

void MazeWidget::generatePath()
{
    int width = m_pathData.m_iWidth;
    int height = m_pathData.m_iHeight;

    assert(width >= 2 && height >= 2);

    m_path = QImage(width * m_iSpacing + 1, height * m_iSpacing + 1, QImage::Format_ARGB32);
    m_path.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&m_path);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for (int i = 0; i < width * height; i++)
    {
        int x = i % width;
        int y = i / width;

        if (m_pathData.m_vVertexes[i].count(y * width + x + 1) == 1)
        {
            painter.drawLine((x + 0.5) * m_iSpacing, (y + 0.5) * m_iSpacing, (x + 1.5) * m_iSpacing, (y + 0.5) * m_iSpacing);
        }
        if (m_pathData.m_vVertexes[i].count((y + 1) * width + x) == 1)
        {
            painter.drawLine((x + 0.5) * m_iSpacing, (y + 0.5) * m_iSpacing, (x + 0.5) * m_iSpacing, (y + 1.5) * m_iSpacing);
        }
    }

    painter.end();
}

void MazeWidget::generateWall()
{
    int width = m_pathData.m_iWidth;
    int height = m_pathData.m_iHeight;

    assert(width >= 2 && height >= 2);

    m_wall = QImage(width * m_iSpacing + 1, height * m_iSpacing + 1, QImage::Format_ARGB32);
    m_wall.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&m_wall);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for (int i = 0; i < width * height; i++)
    {
        int x = i % width;
        int y = i / width;

        if (x == 0)
        {
            painter.drawLine(0, y * m_iSpacing, 0, (y + 1) * m_iSpacing);
        }
        if (y == 0)
        {
            painter.drawLine(x * m_iSpacing, 0, (x + 1) * m_iSpacing, 0);
        }

        if (m_pathData.m_vVertexes[i].count(i + 1) == 0)
        {
            painter.drawLine((x + 1) * m_iSpacing, y * m_iSpacing, (x + 1) * m_iSpacing, (y + 1) * m_iSpacing);
        }
        if (m_pathData.m_vVertexes[i].count(i + width) == 0)
        {
            painter.drawLine(x * m_iSpacing, (y + 1) * m_iSpacing, (x + 1) * m_iSpacing, (y + 1) * m_iSpacing);
        }
    }

    painter.end();
}
