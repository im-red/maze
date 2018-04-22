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
    , m_adjList(-1, -1)
{

}

void MazeWidget::setAdjList(AdjacencyList &list)
{
    m_adjList = list;

    int width = m_adjList.m_iWidth;
    int height = m_adjList.m_iHeight;

    assert(width >= 2 && height >= 2);

    adjustSpacing(width, height);

    generatePath();
    generateWall();

    refreshShowWhat(m_iShowWhat);
}

void MazeWidget::setSolutionList(SolutionList &list)
{
    m_solutionList = list;

    generateSolution();
    generateAccessed();

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

    int width = m_adjList.m_iWidth;
    int height = m_adjList.m_iHeight;

    if (width == -1 && height == -1)
    {
        return;
    }

    m_show = QImage(width * m_iSpacing + 1, height * m_iSpacing + 1, QImage::Format_ARGB32);
    m_show.fill(Qt::white);

    QPainter painter(&m_show);
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);

    if (m_iShowWhat & E_PATH)
    {
        painter.drawImage(0, 0, m_path);
    }
    if (m_iShowWhat & E_WALL)
    {
        painter.drawImage(0, 0, m_wall);
    }
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    if (m_iShowWhat & E_ACCESSED)
    {
        painter.drawImage(0, 0, m_accessed);
    }
    if (m_iShowWhat & E_SOLUTION)
    {
        painter.drawImage(0, 0, m_solution);
    }

    painter.end();
    setPixmap(QPixmap::fromImage(m_show));
}

void MazeWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    m_show.fill(Qt::white);

    int width = m_adjList.m_iWidth;
    int height = m_adjList.m_iHeight;
    if (width != -1 && height != -1)
    {
        adjustSpacing(width, height);
        generatePath();
        generateWall();
        generateSolution();
        generateAccessed();
        refreshShowWhat(m_iShowWhat);
    }
}

void MazeWidget::initImage(QImage &image)
{
    int width = m_adjList.m_iWidth;
    int height = m_adjList.m_iHeight;

    image = QImage(width * m_iSpacing + 1, height * m_iSpacing + 1, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
}

void MazeWidget::drawEdge(QPainter &painter, int p, int q)
{
    int x1 = p % m_adjList.m_iWidth;
    int y1 = p / m_adjList.m_iWidth;
    int x2 = q % m_adjList.m_iWidth;
    int y2 = q / m_adjList.m_iWidth;

    painter.drawLine((x1 + 0.5) * m_iSpacing, (y1 + 0.5) * m_iSpacing, (x2 + 0.5) * m_iSpacing, (y2 + 0.5) * m_iSpacing);
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
    int width = m_adjList.m_iWidth;
    int height = m_adjList.m_iHeight;

    assert(width >= 2 && height >= 2);

    initImage(m_path);

    QPainter painter;
    painter.begin(&m_path);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for (int i = 0; i < width * height; i++)
    {
        if (count(m_adjList.m_vVertexes[i].begin(), m_adjList.m_vVertexes[i].end(), i + 1) == 1)
        {
            drawEdge(painter, i, i + 1);
        }
        if (count(m_adjList.m_vVertexes[i].begin(), m_adjList.m_vVertexes[i].end(), i + width) == 1)
        {
            drawEdge(painter, i, i + width);
        }
    }

    painter.end();
}

void MazeWidget::generateWall()
{
    int width = m_adjList.m_iWidth;
    int height = m_adjList.m_iHeight;

    assert(width >= 2 && height >= 2);

    initImage(m_wall);

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

        if (count(m_adjList.m_vVertexes[i].begin(), m_adjList.m_vVertexes[i].end(), i + 1) == 0)
        {
            painter.drawLine((x + 1) * m_iSpacing, y * m_iSpacing, (x + 1) * m_iSpacing, (y + 1) * m_iSpacing);
        }
        if (count(m_adjList.m_vVertexes[i].begin(), m_adjList.m_vVertexes[i].end(), i + width) == 0)
        {
            painter.drawLine(x * m_iSpacing, (y + 1) * m_iSpacing, (x + 1) * m_iSpacing, (y + 1) * m_iSpacing);
        }
    }

    painter.end();
}

void MazeWidget::generateSolution()
{
    initImage(m_solution);

    QPainter painter;
    painter.begin(&m_solution);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter.setPen(pen);

    for (auto &&edge : m_solutionList.m_vSolution)
    {
        drawEdge(painter, edge.first, edge.second);
    }

    painter.end();
}

void MazeWidget::generateAccessed()
{
    initImage(m_accessed);

    QPainter painter;
    painter.begin(&m_accessed);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::blue);
    painter.setPen(pen);

    for (auto &&edge : m_solutionList.m_vAccessed)
    {
        drawEdge(painter, edge.first, edge.second);
    }

    painter.end();
}
