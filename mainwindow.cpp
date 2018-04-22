#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deepfirstsearch.h"
#include "kruskal.h"
#include "prim.h"
#include "recursivedivision.h"

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
    , m_adjList(0, 0)
{
    ui->setupUi(this);

    foreach (QAction *action, ui->menuGen->actions())
    {
        m_genGroup.addAction(action);
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

void MainWindow::on_okButton_clicked()
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
