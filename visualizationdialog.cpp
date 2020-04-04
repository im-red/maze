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

#include "visualizationdialog.h"
#include "ui_visualizationdialog.h"

VisualizationDialog::VisualizationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VisualizationDialog)
{
    ui->setupUi(this);
    setWindowTitle("Visualization");

    ui->vWidget->setStep(ui->sbStep->value());
    ui->vWidget->setFps(ui->sbFps->value());
}

VisualizationDialog::~VisualizationDialog()
{
    delete ui;
}

void VisualizationDialog::setAdjacencyList(const AdjacencyList &list)
{
    ui->vWidget->setAdjacencyList(list);
    ui->vWidget->reset();
}

void VisualizationDialog::on_buttonPlay_clicked()
{
    QString s = ui->buttonPlay->text();
    if (s == "Play")
    {
        ui->buttonPlay->setText("Pause");
        ui->vWidget->play();
        ui->buttonPrev->setEnabled(false);
        ui->buttonNext->setEnabled(false);
    }
    else
    {
        ui->buttonPlay->setText("Play");
        ui->vWidget->pause();
        ui->buttonPrev->setEnabled(true);
        ui->buttonNext->setEnabled(true);
    }
}

void VisualizationDialog::on_buttonReset_clicked()
{
    ui->vWidget->reset();
}

void VisualizationDialog::on_buttonNext_clicked()
{
    ui->vWidget->nextFrame();
}

void VisualizationDialog::on_buttonPrev_clicked()
{
    ui->vWidget->prevFrame();
}

void VisualizationDialog::on_sbStep_valueChanged(int value)
{
    ui->vWidget->setStep(value);
}

void VisualizationDialog::on_sbFps_valueChanged(int value)
{
    ui->vWidget->setFps(value);
}
