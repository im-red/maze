#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mazewidget.h"
#include "adjacencylist.h"
#include "solutionlist.h"

#include <QMainWindow>
#include <QActionGroup>

#include <vector>

class QLabel;

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
    void on_buttonGenerate_clicked();
    void on_buttonSolve_clicked();
    void on_saveButton_clicked();
    void on_checkBoxWall_toggled(bool checked);
    void on_checkBoxPath_toggled(bool checked);
    void on_checkBoxSolution_toggled(bool checked);
    void on_checkBoxAccessed_toggled(bool checked);

private:
    void doGenerate();
    void doSolve();

    Ui::MainWindow *ui;

    int m_showWhat;
    QActionGroup m_genGroup;
    QActionGroup m_solveGroup;
    AdjacencyList m_adjList;
    SolutionList m_solutionList;
    std::vector<QLabel *> m_nodeNumLabels;
    std::vector<QLabel *> m_nodePercentLabels;
};

#endif // MAINWINDOW_H
