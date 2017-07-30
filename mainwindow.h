#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mazewidget.h"

#include <QMainWindow>

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
    void generate(int row, int column);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
