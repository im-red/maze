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
    enum E_SHOW_WHAT
    {
        E_PATH = 1,
        E_WALL = 1 << 1,
        E_SOLUTION = 1 << 2,
        E_ACCESSED = 1 << 3
    };
    explicit MazeWidget(QWidget *parent = nullptr);
    void setAdjList(AdjacencyList &list);
    void setSolutionList(SolutionList &list);
    void setShowWhat(int showWhat) { m_iShowWhat = showWhat; }

signals:

public slots:
    void save();
    void refreshShowWhat(int showWhat);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void initImage(QImage &image);
    void drawEdge(QPainter &painter, int p, int q);
    void adjustSpacing(int width, int height);
    void generatePath();
    void generateWall();
    void generateSolution();
    void generateAccessed();

private:
    int m_iShowWhat;
    AdjacencyList m_adjList;
    SolutionList m_solutionList;

    int m_iPicSpacing = 50;
    int m_iSpacing = 50;
    int m_iMinSpacing = 10;

    QImage m_wall;
    QImage m_path;
    QImage m_solution;
    QImage m_accessed;
    QImage m_show;
};

#endif // MAZEWIDGET_H
