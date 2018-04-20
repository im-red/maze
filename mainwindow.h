#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mazewidget.h"

#include <QMainWindow>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_okButton_clicked();
    void on_saveButton_clicked();
    void on_actionWall_toggled(bool checked);
    void on_actionPath_toggled(bool checked);
    void generateDFS(int row, int column);

private:
    Ui::MainWindow *ui;

    int m_iShowWhat;
    QActionGroup m_genGroup;
};

#endif // MAINWINDOW_H
