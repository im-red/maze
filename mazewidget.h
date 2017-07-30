#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include "mazedata.h"

#include <QWidget>
#include <QPaintEvent>

class MazeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MazeWidget(QWidget *parent = nullptr);
    void setData(MazeData &data) { m_data = data; }

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    MazeData m_data;
    int m_spacing = 50;
    int m_minSpacing = 10;
};

#endif // MAZEWIDGET_H
