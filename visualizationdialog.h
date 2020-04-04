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

#include "adjacencylist.h"

#include <QDialog>

namespace Ui {
class VisualizationDialog;
}

class VisualizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VisualizationDialog(QWidget *parent = nullptr);
    ~VisualizationDialog() override;

    void setAdjacencyList(const AdjacencyList &list);

private slots:
    void on_buttonPlay_clicked();
    void on_buttonReset_clicked();
    void on_buttonNext_clicked();
    void on_buttonPrev_clicked();
    void on_sbStep_valueChanged(int value);
    void on_sbFps_valueChanged(int value);

private:
    Ui::VisualizationDialog *ui;
};

