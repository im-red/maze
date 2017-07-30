#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include "mazedata.h"

#include <QWidget>
#include <QPaintEvent>
#include <QPicture>

class MazeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MazeWidget(QWidget *parent = nullptr);
    void setData(MazeData &data) { m_data = data; }

signals:

public slots:
    void save();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    MazeData m_data;
    int m_iPicSpacing = 50;
    int m_iSpacing = 50;
    int m_iMinSpacing = 10;
    QPicture m_picture;
};

#endif // MAZEWIDGET_H
