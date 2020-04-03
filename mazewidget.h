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

#pragma once

#include "adjacencylist.h"
#include "solutionlist.h"

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPainter>

class QResizeEvent;

class MazeWidget : public QLabel
{
    Q_OBJECT

public:
    enum ShowWhat
    {
        Path = 1,
        Wall = 1 << 1,
        Solution = 1 << 2,
        Accessed = 1 << 3
    };

public:
    explicit MazeWidget(QWidget *parent = nullptr);
    void setAdjacencyList(const AdjacencyList &list);
    void setSolutionList(const SolutionList &list);

public slots:
    void save();
    void updateShowWhat(int showWhat);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void clearImage(QImage &image);
    void drawEdge(QPainter &painter, int p, int q);
    void adjustSpacing(int row, int column);

    void generatePath();
    void generateWall();
    void generateSolution();
    void generateAccessed();

private:
    int m_showWhat;
    AdjacencyList m_adjacencyList;
    SolutionList m_solutionList;

    const int m_pictureMargin;
    const int m_minSpacing;
    int m_spacing;

    QImage m_wall;
    QImage m_path;
    QImage m_solution;
    QImage m_accessed;
    QImage m_show;
};
