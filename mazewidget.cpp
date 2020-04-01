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
    , m_showWhat(Path)
    , m_adjacencyList(-1, -1)
{

}

void MazeWidget::setAdjacencyList(AdjacencyList &list)
{
    m_adjacencyList = list;

    int column = m_adjacencyList.m_column;
    int row = m_adjacencyList.m_row;

    assert(row >= 2 && column >= 2);

    adjustSpacing(row, column);

    generatePath();
    generateWall();

    refreshShowWhat(m_showWhat);
}

void MazeWidget::setSolutionList(SolutionList &list)
{
    m_solutionList = list;

    generateSolution();
    generateAccessed();

    refreshShowWhat(m_showWhat);
}

void MazeWidget::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, "", "", tr("Image Files (*.png)"));
    if (fileName.isEmpty())
    {
        return;
    }
    //QPixmap pixmap(this->size());
    QPixmap pixmap(this->width() + 2 * m_pictureMargin, this->height() + 2 * m_pictureMargin);
    this->render(&pixmap, QPoint(m_pictureMargin, m_pictureMargin));
    pixmap.save(fileName, "png");
}

void MazeWidget::refreshShowWhat(int showWhat)
{
    m_showWhat = showWhat;

    int column = m_adjacencyList.m_column;
    int row = m_adjacencyList.m_row;

    if (row == -1 || column == -1)
    {
        return;
    }

    m_show = QImage(column * m_spacing + 1, row * m_spacing + 1, QImage::Format_ARGB32);
    m_show.fill(Qt::white);

    QPainter painter(&m_show);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::green);
    pen.setWidth(3);
    painter.setPen(pen);

    painter.drawLine(0.5 * m_spacing, 0, 0.5 * m_spacing, m_spacing);
    painter.drawLine(0.5 * m_spacing, m_spacing, 0, 0.5 * m_spacing);
    painter.drawLine(0.5 * m_spacing, m_spacing, m_spacing, 0.5 * m_spacing);

    painter.drawLine((0.5 + column - 1) * m_spacing, (row - 1) * m_spacing, (0.5 + column - 1) * m_spacing, row * m_spacing);
    painter.drawLine((0.5 + column - 1) * m_spacing, row * m_spacing, (column - 1) * m_spacing, (0.5 + row - 1) * m_spacing);
    painter.drawLine((0.5 + column - 1) * m_spacing, row * m_spacing, column * m_spacing, (0.5 + row - 1) * m_spacing);

    painter.setCompositionMode(QPainter::CompositionMode_Multiply);

    if (m_showWhat & Path)
    {
        painter.drawImage(0, 0, m_path);
    }
    if (m_showWhat & Wall)
    {
        painter.drawImage(0, 0, m_wall);
    }
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    if (m_showWhat & Accessed)
    {
        painter.drawImage(0, 0, m_accessed);
    }
    if (m_showWhat & Solution)
    {
        painter.drawImage(0, 0, m_solution);
    }

    painter.end();
    setPixmap(QPixmap::fromImage(m_show));
}

void MazeWidget::resizeEvent(QResizeEvent *)
{
    m_show.fill(Qt::white);

    int column = m_adjacencyList.m_column;
    int row = m_adjacencyList.m_row;

    if (row != -1 && column != -1)
    {
        adjustSpacing(row, column);
        generatePath();
        generateWall();
        generateSolution();
        generateAccessed();
        refreshShowWhat(m_showWhat);
    }
}

void MazeWidget::clearImage(QImage &image)
{
    int column = m_adjacencyList.m_column;
    int row = m_adjacencyList.m_row;

    image = QImage(column * m_spacing + 1, row * m_spacing + 1, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
}

void MazeWidget::drawEdge(QPainter &painter, int p, int q)
{
    int x1 = p % m_adjacencyList.m_column;
    int y1 = p / m_adjacencyList.m_column;
    int x2 = q % m_adjacencyList.m_column;
    int y2 = q / m_adjacencyList.m_column;

    painter.drawLine((x1 + 0.5) * m_spacing, (y1 + 0.5) * m_spacing, (x2 + 0.5) * m_spacing, (y2 + 0.5) * m_spacing);
}

void MazeWidget::adjustSpacing(int row, int column)
{
    int rowSpacing = (this->parentWidget()->height() - 1) / row;
    int columnSpacing = (this->parentWidget()->width() - 1) / column;
    m_spacing = rowSpacing > columnSpacing ? columnSpacing : rowSpacing;
    m_spacing = m_spacing > m_minSpacing ? m_spacing : m_minSpacing;
}

void MazeWidget::generatePath()
{
    int column = m_adjacencyList.m_column;
    int row = m_adjacencyList.m_row;

    assert(row >= 2 && column >= 2);

    clearImage(m_path);

    QPainter painter;
    painter.begin(&m_path);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for (int i = 0; i < row * column; i++)
    {
        if (count(m_adjacencyList.m_nodes[i].begin(), m_adjacencyList.m_nodes[i].end(), i + 1) == 1)
        {
            drawEdge(painter, i, i + 1);
        }
        if (count(m_adjacencyList.m_nodes[i].begin(), m_adjacencyList.m_nodes[i].end(), i + column) == 1)
        {
            drawEdge(painter, i, i + column);
        }
    }

    painter.end();
}

void MazeWidget::generateWall()
{
    int column = m_adjacencyList.m_column;
    int row = m_adjacencyList.m_row;

    assert(row >= 2 && column >= 2);

    clearImage(m_wall);

    QPainter painter;
    painter.begin(&m_wall);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for (int i = 0; i < row * column; i++)
    {
        int x = i % column;
        int y = i / column;

        if (x == 0)
        {
            painter.drawLine(0, y * m_spacing, 0, (y + 1) * m_spacing);
        }
        if (y == 0)
        {
            painter.drawLine(x * m_spacing, 0, (x + 1) * m_spacing, 0);
        }

        if (count(m_adjacencyList.m_nodes[i].begin(), m_adjacencyList.m_nodes[i].end(), i + 1) == 0)
        {
            painter.drawLine((x + 1) * m_spacing, y * m_spacing, (x + 1) * m_spacing, (y + 1) * m_spacing);
        }
        if (count(m_adjacencyList.m_nodes[i].begin(), m_adjacencyList.m_nodes[i].end(), i + column) == 0)
        {
            painter.drawLine(x * m_spacing, (y + 1) * m_spacing, (x + 1) * m_spacing, (y + 1) * m_spacing);
        }
    }

    painter.end();
}

void MazeWidget::generateSolution()
{
    clearImage(m_solution);

    QPainter painter;
    painter.begin(&m_solution);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter.setPen(pen);

    for (auto &&edge : m_solutionList.m_solution)
    {
        drawEdge(painter, edge.first, edge.second);
    }

    painter.end();
}

void MazeWidget::generateAccessed()
{
    clearImage(m_accessed);

    QPainter painter;
    painter.begin(&m_accessed);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::blue);
    painter.setPen(pen);

    for (auto &&edge : m_solutionList.m_accessed)
    {
        drawEdge(painter, edge.first, edge.second);
    }

    painter.end();
}
