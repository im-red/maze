#include "mazewidget.h"

#include <QPainter>
#include <QPen>
#include <iostream>
#include <QDebug>
#include <QPixmap>
#include <QFileDialog>
#include <QResizeEvent>

#include <cassert>
#include <algorithm>

using namespace std;

MazeWidget::MazeWidget(QWidget *parent)
    : QLabel(parent)
    , m_iShowWhat(E_PATH)
    , m_pathData(-1, -1)
{

}

void MazeWidget::setAdjList(AdjacencyList &list)
{
    m_pathData = list;

    int width = m_pathData.m_iWidth;
    int height = m_pathData.m_iHeight;

    assert(width >= 2 && height >= 2);

    adjustSpacing(width, height);

    generatePath();
    generateWall();

    refreshShowWhat(m_iShowWhat);
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

void MazeWidget::refreshShowWhat(int showWhat)
{
    m_iShowWhat = showWhat;

    int width = m_pathData.m_iWidth;
    int height = m_pathData.m_iHeight;

    m_show = QImage(width * m_iSpacing + 1, height * m_iSpacing + 1, QImage::Format_ARGB32);
    m_show.fill(Qt::white);

    QPainter painter(&m_show);
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);

    if (m_iShowWhat & E_PATH)
    {
        painter.drawImage(0, 0, m_path);
    }
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    if (m_iShowWhat & E_WALL)
    {
        painter.drawImage(0, 0, m_wall);
    }

    painter.end();
    setPixmap(QPixmap::fromImage(m_show));
}

void MazeWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    m_show.fill(Qt::white);

    int width = m_pathData.m_iWidth;
    int height = m_pathData.m_iHeight;
    if (width != -1 && height != -1)
    {
        setAdjList(m_pathData);
        refreshShowWhat(m_iShowWhat);
    }
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

        if (count(m_pathData.m_vVertexes[i].begin(), m_pathData.m_vVertexes[i].end(), y * width + x + 1) == 1)
        {
            painter.drawLine((x + 0.5) * m_iSpacing, (y + 0.5) * m_iSpacing, (x + 1.5) * m_iSpacing, (y + 0.5) * m_iSpacing);
        }
        if (count(m_pathData.m_vVertexes[i].begin(), m_pathData.m_vVertexes[i].end(), (y + 1) * width + x) == 1)
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

        if (count(m_pathData.m_vVertexes[i].begin(), m_pathData.m_vVertexes[i].end(), i + 1) == 0)
        {
            painter.drawLine((x + 1) * m_iSpacing, y * m_iSpacing, (x + 1) * m_iSpacing, (y + 1) * m_iSpacing);
        }
        if (count(m_pathData.m_vVertexes[i].begin(), m_pathData.m_vVertexes[i].end(), i + width) == 0)
        {
            painter.drawLine(x * m_iSpacing, (y + 1) * m_iSpacing, (x + 1) * m_iSpacing, (y + 1) * m_iSpacing);
        }
    }

    painter.end();
}
