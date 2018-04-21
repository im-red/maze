#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include "adjacencylist.h"

#include <QWidget>
#include <QLabel>
#include <QImage>

class QResizeEvent;

class MazeWidget : public QLabel
{
    Q_OBJECT
public:
    enum E_SHOW_WHAT
    {
        E_PATH = 1,
        E_WALL = 1 << 1
    };
    explicit MazeWidget(QWidget *parent = nullptr);
    void setPath(AdjacencyList &list);
    void setShowWhat(int showWhat) { m_iShowWhat = showWhat; }

signals:

public slots:
    void save();
    void refreshShowWhat(int showWhat);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void adjustSpacing(int width, int height);
    void generatePath();
    void generateWall();

private:
    int m_iShowWhat;
    AdjacencyList m_pathData;

    int m_iPicSpacing = 50;
    int m_iSpacing = 50;
    int m_iMinSpacing = 10;

    QImage m_wall;
    QImage m_path;
    QImage m_show;
};

#endif // MAZEWIDGET_H
