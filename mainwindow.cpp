#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deepfirstsearch.h"
#include "kruskal.h"
#include "prim.h"
#include "recursivedivision.h"
#include "wallfollower.h"

#include <QPainter>
#include <QDebug>
#include <iostream>
#include <ctime>
#include <random>
#include <vector>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_iShowWhat(0)
    , m_genGroup(this)
    , m_solveGroup(this)
    , m_adjList(-1, -1)
{
    ui->setupUi(this);

    foreach (QAction *action, ui->menuGen->actions())
    {
        m_genGroup.addAction(action);
    }
    m_genGroup.setExclusive(true);

    foreach (QAction *action, ui->menuSolve->actions())
    {
        m_solveGroup.addAction(action);
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

    m_iShowWhat = ui->actionWall->isChecked() ? MazeWidget::E_WALL : 0 + ui->actionPath->isChecked() ? MazeWidget::E_PATH : 0;
    ui->mazeWidget->setShowWhat(m_iShowWhat);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonSolve_clicked()
{
    doSolve();
    ui->actionSolution->setChecked(true);
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

void MainWindow::on_actionWall_toggled(bool checked)
{
    Q_UNUSED(checked)

    m_iShowWhat ^= MazeWidget::E_WALL;
    ui->mazeWidget->refreshShowWhat(m_iShowWhat);
}

void MainWindow::on_actionPath_toggled(bool checked)
{
    Q_UNUSED(checked)

    m_iShowWhat ^= MazeWidget::E_PATH;
    ui->mazeWidget->refreshShowWhat(m_iShowWhat);
}

void MainWindow::on_actionSolution_toggled(bool checked)
{
    Q_UNUSED(checked)

    m_iShowWhat ^= MazeWidget::E_SOLUTION;
    ui->mazeWidget->refreshShowWhat(m_iShowWhat);
}

void MainWindow::on_actionAccessed_toggled(bool checked)
{
    Q_UNUSED(checked)

    m_iShowWhat ^= MazeWidget::E_ACCESSED;
    ui->mazeWidget->refreshShowWhat(m_iShowWhat);
}

void MainWindow::doGenerate()
{
    int width = ui->widthSpinBox->value();
    int height = ui->heightSpinBox->value();

    if (ui->actionDFS->isChecked())
    {
        DeepFirstSearch dfs(width, height);
        m_adjList = dfs.generate();
    }
    else if (ui->actionKruskal->isChecked())
    {
        Kruskal kruskal(width, height);
        m_adjList = kruskal.generate();
    }
    else if (ui->actionPrim->isChecked())
    {
        Prim prim(width, height);
        m_adjList = prim.generate();
    }
    else if (ui->actionDiv->isChecked())
    {
        RecursiveDivision div(width, height);
        m_adjList = div.generate();
    }

    vector<int> stat = AdjacencyList::neighborStat(m_adjList);
    for (int i = 0; i < stat.size(); i++)
    {
        m_nodeNumLabels[i]->setText(QString::number(stat[i]));
        m_nodePercentLabels[i]->setText(QString::number(stat[i] * 100.0 / (m_adjList.m_iWidth * m_adjList.m_iHeight)));
    }
    ui->mazeWidget->setAdjList(m_adjList);
}

void MainWindow::doSolve()
{
    if (m_adjList.m_iWidth == -1 || m_adjList.m_iHeight == -1)
    {
        return;
    }

    if (ui->actionLeftHand->isChecked())
    {
        WallFollower wf(WallFollower::E_LEFT_HAND);
        m_solutionList = wf.solve(m_adjList);
    }
    else if (ui->actionRightHand->isChecked())
    {
        WallFollower wf(WallFollower::E_RIGHT_HAND);
        m_solutionList = wf.solve(m_adjList);
    }

    size_t num = 0;
    num = m_solutionList.m_vSolution.size() + 1;
    ui->labelSolutionNode->setText(num != 0 ? QString::number(num) : QString("/"));
    ui->labelSolutionNodePercent->setText(num != 0 ? QString::number(num * 100.0 / (m_adjList.m_iWidth * m_adjList.m_iHeight)) : QString("/"));

    num = m_solutionList.m_vAccessed.size() + 1;
    ui->labelAccessedNode->setText(num != 0 ? QString::number(num) : QString("/"));
    ui->labelAccessedNodePercent->setText(num != 0 ? QString::number(num * 100.0 / (m_adjList.m_iWidth * m_adjList.m_iHeight)) : QString("/"));

    num = m_solutionList.m_vTrace.size() + 1;
    ui->labelTraceNode->setText(num != 0 ? QString::number(num) : QString("/"));
    ui->labelTraceNodePercent->setText(num != 0 ? QString::number(num * 100.0 / (m_adjList.m_iWidth * m_adjList.m_iHeight)) : QString("/"));

    ui->mazeWidget->setSolutionList(m_solutionList);
}
