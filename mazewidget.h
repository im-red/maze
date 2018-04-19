#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include "mazedata.h"
#include "adjacencylist.h"

#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QPicture>
#include <QImage>

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
    void setWall(MazeData &data);
    void setPath(AdjacencyList &list);
    void setShowWhat(E_SHOW_WHAT what) { m_iShowWhat = what; }

signals:

public slots:
    void save();

private:
    void adjustSpacing(int width, int height);
    void generatePath();
    void generateWall();

private:
    int m_iShowWhat;

    MazeData m_wallData;
    AdjacencyList m_pathData;

    int m_iPicSpacing = 50;
    int m_iSpacing = 50;
    int m_iMinSpacing = 10;

    QImage m_wall;
    QImage m_path;
};

#endif // MAZEWIDGET_H