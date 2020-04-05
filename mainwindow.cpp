/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2020 Jia Lihong
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deepfirstsearch.h"
#include "kruskal.h"
#include "prim.h"
#include "recursivedivision.h"
#include "wallfollower.h"
#include "astar.h"
#include "visualizationdialog.h"
#include "util.h"

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
    , m_genGroup(this)
    , m_solveGroup(this)
    , m_adjacencyList(-1, -1)
    , m_vDialog(new VisualizationDialog)
{
    srand(static_cast<unsigned>(time(nullptr)));
    ui->setupUi(this);

    initUI();
    initConnection();

    updateShowWhat();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonGenerate_clicked()
{
    StopWatch sw(__FUNCTION__);

    doGenerate();
    doSolve();
    ui->buttonV->setEnabled(true);
}

void MainWindow::on_buttonV_clicked()
{
    m_vDialog->setAdjacencyList(m_adjacencyList);
    m_vDialog->exec();
}

void MainWindow::on_actionSave_triggered()
{
    ui->mazeWidget->save();
}

void MainWindow::updateShowWhat()
{
    int showWhat = (ui->checkBoxWall->isChecked() ? MazeWidget::Wall : 0)
                 + (ui->checkBoxPath->isChecked() ? MazeWidget::Path : 0)
                 + (ui->checkBoxSolution->isChecked() ? MazeWidget::Solution : 0)
                 + (ui->checkBoxAccessed->isChecked() ? MazeWidget::Accessed : 0);
    ui->mazeWidget->updateShowWhat(showWhat);
}

void MainWindow::initConnection()
{
    connect(ui->checkBoxWall, &QCheckBox::toggled, this, &MainWindow::updateShowWhat);
    connect(ui->checkBoxPath, &QCheckBox::toggled, this, &MainWindow::updateShowWhat);
    connect(ui->checkBoxSolution, &QCheckBox::toggled, this, &MainWindow::updateShowWhat);
    connect(ui->checkBoxAccessed, &QCheckBox::toggled, this, &MainWindow::updateShowWhat);
}

void MainWindow::initUI()
{
    foreach (QAction *action, ui->menuGen->actions())
    {
        m_genGroup.addAction(action);
    }
    m_genGroup.setExclusive(true);

    foreach (QAction *action, ui->menuSolve->actions())
    {
        m_solveGroup.addAction(action);
        connect(action, &QAction::triggered, this, &MainWindow::doSolve);
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
}

void MainWindow::doGenerate()
{
    StopWatch sw(__FUNCTION__);

    int column = ui->columnSpinBox->value();
    int row = ui->rowSpinBox->value();

    if (ui->actionDFS->isChecked())
    {
        DeepFirstSearch dfs(row, column);
        m_adjacencyList = dfs.generate();
    }
    else if (ui->actionKruskal->isChecked())
    {
        Kruskal kruskal(row, column);
        m_adjacencyList = kruskal.generate();
    }
    else if (ui->actionPrim->isChecked())
    {
        Prim prim(row, column);
        m_adjacencyList = prim.generate();
    }
    else if (ui->actionDiv->isChecked())
    {
        RecursiveDivision div(row, column);
        m_adjacencyList = div.generate();
    }

    ui->mazeWidget->setAdjacencyList(m_adjacencyList);

    updateMazeStat();
}

void MainWindow::doSolve()
{
    StopWatch sw(__FUNCTION__);

    if (!m_adjacencyList.valid())
    {
        qInfo() << "adjacency list is invalid";
        return;
    }

    if (ui->actionLeftHand->isChecked())
    {
        WallFollower wf(WallFollower::LeftHand);
        m_solutionList = wf.solve(m_adjacencyList);
    }
    else if (ui->actionRightHand->isChecked())
    {
        WallFollower wf(WallFollower::RightHand);
        m_solutionList = wf.solve(m_adjacencyList);
    }
    else if (ui->actionManhattan->isChecked())
    {
        AStar as(AStar::Manhattan);
        m_solutionList = as.solve(m_adjacencyList);
    }
    else if (ui->actionEuclidian->isChecked())
    {
        AStar as(AStar::Euclidian);
        m_solutionList = as.solve(m_adjacencyList);
    }
    else if (ui->actionZero->isChecked())
    {
        AStar as(AStar::Zero);
        m_solutionList = as.solve(m_adjacencyList);
    }

    ui->mazeWidget->setSolutionList(m_solutionList);

    updateSolutionStat();
}

void MainWindow::updateMazeStat()
{
    vector<int> stat = m_adjacencyList.neighborStat();
    for (size_t i = 0; i < stat.size(); i++)
    {
        m_nodeNumLabels[i]->setText(QString::number(stat[i]));
        m_nodePercentLabels[i]->setText(QString::number(stat[i] * 100.0 / (m_adjacencyList.nodeCount())));
    }
}

void MainWindow::updateSolutionStat()
{
    int total = m_adjacencyList.nodeCount();
    size_t num = 0;

    num = m_solutionList.m_solution.size();
    ui->labelSolutionNode->setText(num ? QString::number(num + 1) : QString("/"));
    ui->labelSolutionNodePercent->setText(num ? QString::number((num + 1) * 100.0 / total) : QString("/"));

    num = m_solutionList.m_accessed.size();
    ui->labelAccessedNode->setText(num ? QString::number(num + 1) : QString("/"));
    ui->labelAccessedNodePercent->setText(num ? QString::number((num + 1) * 100.0 / total) : QString("/"));

    num = m_solutionList.m_trace.size();
    ui->labelTraceNode->setText(num ? QString::number(num + 1) : QString("/"));
    ui->labelTraceNodePercent->setText(num ? QString::number((num + 1) * 100.0 / total) : QString("/"));
}
