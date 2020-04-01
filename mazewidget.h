#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

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

    int m_pictureMargin = 50;
    int m_spacing = 50;
    int m_minSpacing = 10;

    QImage m_wall;
    QImage m_path;
    QImage m_solution;
    QImage m_accessed;
    QImage m_show;
};

#endif // MAZEWIDGET_H
