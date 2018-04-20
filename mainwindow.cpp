#include "unionfind.h"
#include "mazedata.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deepfirstsearch.h"
#include "adjacencylist.h"

#include <QVector>
#include <QBitArray>
#include <QPainter>
#include <QDebug>
#include <iostream>
#include <ctime>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_iShowWhat(0)
    , m_genGroup(this)
{
    ui->setupUi(this);

    foreach (QAction *action, ui->menuGen->actions())
    {
        m_genGroup.addAction(action);
    }
    m_genGroup.setExclusive(true);

    m_iShowWhat = ui->actionWall->isChecked() ? MazeWidget::E_WALL : 0 + ui->actionPath->isChecked() ? MazeWidget::E_PATH : 0;
    ui->mazeWidget->setShowWhat(m_iShowWhat);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_okButton_clicked()
{
    if (ui->actionDFS->isChecked())
    {
        generateDFS(ui->rowSpinBox->value(), ui->colSpinBox->value());
    }
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

void MainWindow::generateDFS(int row, int column)
{

    DeepFirstSearch dfs(column, row);
    AdjacencyList list = dfs.generate();

    ui->mazeWidget->setPath(list);
}
