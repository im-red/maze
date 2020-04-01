#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deepfirstsearch.h"
#include "kruskal.h"
#include "prim.h"
#include "recursivedivision.h"
#include "wallfollower.h"
#include "astar.h"

#include <QPainter>
#include <QDebug>
#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include <cstdlib>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_showWhat(0)
    , m_genGroup(this)
    , m_solveGroup(this)
    , m_adjList(-1, -1)
{
    srand(time(nullptr));
    ui->setupUi(this);

    foreach (QAction *action, ui->menuGen->actions())
    {
        m_genGroup.addAction(action);
    }
    m_genGroup.setExclusive(true);

    foreach (QAction *action, ui->menuSolve->actions())
    {
        m_solveGroup.addAction(action);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(on_buttonSolve_clicked()));
    }
    m_genGroup.setExclusive(true);

    m_nodeNumLabels.push_back(ui->label1neighbor);
    m_nodeNumLabels.push_back(ui->label2neighbor);
    m_nodeNumLabels.push_back(ui->label3neighbor);
    m_nodeNumLabels.push_back(ui->label4neighbor);

    m_nodePercentLabels.push_back(ui->label1neighborpercent);
    m_nodePercentLabels.push_back(ui->label2neighborpercent);
    m_nodePercentLabels.push_back(ui->label3neighborpercent);
    m_nodePercentLabels.push_back(ui->label4neighborpercent);

    m_showWhat = ui->actionWall->isChecked() ? MazeWidget::Wall : 0 + ui->actionPath->isChecked() ? MazeWidget::Path : 0;
    ui->mazeWidget->setShowWhat(m_showWhat);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonSolve_clicked()
{
    doSolve();
    ui->checkBoxSolution->setChecked(true);
}

void MainWindow::on_buttonGenerate_clicked()
{
    doGenerate();
    doSolve();
}

void MainWindow::on_saveButton_clicked()
{
    ui->mazeWidget->save();
}

void MainWindow::on_checkBoxWall_toggled(bool checked)
{
    Q_UNUSED(checked)

    m_showWhat ^= MazeWidget::Wall;
    ui->mazeWidget->refreshShowWhat(m_showWhat);
}

void MainWindow::on_checkBoxPath_toggled(bool checked)
{
    Q_UNUSED(checked)

    m_showWhat ^= MazeWidget::Path;
    ui->mazeWidget->refreshShowWhat(m_showWhat);
}

void MainWindow::on_checkBoxSolution_toggled(bool checked)
{
    Q_UNUSED(checked)

    m_showWhat ^= MazeWidget::Solution;
    ui->mazeWidget->refreshShowWhat(m_showWhat);
}

void MainWindow::on_checkBoxAccessed_toggled(bool checked)
{
    Q_UNUSED(checked)

    m_showWhat ^= MazeWidget::Accessed;
    ui->mazeWidget->refreshShowWhat(m_showWhat);
}

void MainWindow::doGenerate()
{
    int column = ui->columnSpinBox->value();
    int row = ui->rowSpinBox->value();

    if (ui->actionDFS->isChecked())
    {
        DeepFirstSearch dfs(row, column);
        m_adjList = dfs.generate();
    }
    else if (ui->actionKruskal->isChecked())
    {
        Kruskal kruskal(row, column);
        m_adjList = kruskal.generate();
    }
    else if (ui->actionPrim->isChecked())
    {
        Prim prim(row, column);
        m_adjList = prim.generate();
    }
    else if (ui->actionDiv->isChecked())
    {
        RecursiveDivision div(row, column);
        m_adjList = div.generate();
    }

    vector<int> stat = AdjacencyList::neighborStat(m_adjList);
    for (int i = 0; i < stat.size(); i++)
    {
        m_nodeNumLabels[i]->setText(QString::number(stat[i]));
        m_nodePercentLabels[i]->setText(QString::number(stat[i] * 100.0 / (m_adjList.m_row * m_adjList.m_column)));
    }
    ui->mazeWidget->setAdjacencyList(m_adjList);
}

void MainWindow::doSolve()
{
    if (m_adjList.m_row == -1 || m_adjList.m_column == -1)
    {
        return;
    }

    if (ui->actionLeftHand->isChecked())
    {
        WallFollower wf(WallFollower::LeftHand);
        m_solutionList = wf.solve(m_adjList);
    }
    else if (ui->actionRightHand->isChecked())
    {
        WallFollower wf(WallFollower::RightHand);
        m_solutionList = wf.solve(m_adjList);
    }
    else if (ui->actionManhattan->isChecked())
    {
        AStar as(AStar::Manhattan);
        m_solutionList = as.solve(m_adjList);
    }
    else if (ui->actionEuclidian->isChecked())
    {
        AStar as(AStar::Euclidian);
        m_solutionList = as.solve(m_adjList);
    }
    else if (ui->actionZero->isChecked())
    {
        AStar as(AStar::Zero);
        m_solutionList = as.solve(m_adjList);
    }

    size_t num = 0;
    num = m_solutionList.m_solution.size() + 1;
    ui->labelSolutionNode->setText(num != 1 ? QString::number(num) : QString("/"));
    ui->labelSolutionNodePercent->setText(num != 1 ? QString::number(num * 100.0 / (m_adjList.m_row * m_adjList.m_column)) : QString("/"));

    num = m_solutionList.m_accessed.size() + 1;
    ui->labelAccessedNode->setText(num != 1 ? QString::number(num) : QString("/"));
    ui->labelAccessedNodePercent->setText(num != 1 ? QString::number(num * 100.0 / (m_adjList.m_row * m_adjList.m_column)) : QString("/"));

    num = m_solutionList.m_trace.size() + 1;
    ui->labelTraceNode->setText(num != 1 ? QString::number(num) : QString("/"));
    ui->labelTraceNodePercent->setText(num != 1 ? QString::number(num * 100.0 / (m_adjList.m_row * m_adjList.m_column)) : QString("/"));

    ui->mazeWidget->setSolutionList(m_solutionList);
}
