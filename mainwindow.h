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

#pragma once

#include "mazewidget.h"
#include "adjacencylist.h"
#include "solutionlist.h"

#include <QMainWindow>
#include <QActionGroup>

#include <vector>

class QLabel;
class VisualizationDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonGenerate_clicked();
    void on_buttonV_clicked();
    void on_actionSave_triggered();
    void updateShowWhat();

private:
    void initConnection();
    void initUI();

    void doGenerate();
    void doSolve();

    void updateMazeStat();
    void updateSolutionStat();

    Ui::MainWindow *ui;

    QActionGroup m_genGroup;
    QActionGroup m_solveGroup;
    AdjacencyList m_adjacencyList;
    SolutionList m_solutionList;
    std::vector<QLabel *> m_nodeNumLabels;
    std::vector<QLabel *> m_nodePercentLabels;

    VisualizationDialog *m_vDialog;
};
