#include "glmazedialog.h"
#include "ui_glmazedialog.h"

#include <QKeyEvent>

GLMazeDialog::GLMazeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GLMazeDialog)
{
    ui->setupUi(this);
    on_sliderAmbient_valueChanged(ui->sliderAmbient->value());
    on_sliderDiffuse_valueChanged(ui->sliderDiffuse->value());
    on_sliderSpecular_valueChanged(ui->sliderSpecular->value());
    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();
}

GLMazeDialog::~GLMazeDialog()
{
    delete ui;
}

void GLMazeDialog::on_sliderAmbient_valueChanged(int value)
{
    float f = value / 100.0;
    ui->labelAmbient->setText(QString::number(f));
    ui->openGLWidget->setAmbientStrength(f);
}

void GLMazeDialog::on_sliderDiffuse_valueChanged(int value)
{
    float f = value / 100.0;
    ui->labelDiffuse->setText(QString::number(f));
    ui->openGLWidget->setDiffuseStrength(f);
}

void GLMazeDialog::on_sliderSpecular_valueChanged(int value)
{
    float f = value / 100.0;
    ui->labelSpecular->setText(QString::number(f));
    ui->openGLWidget->setSpecularStrength(f);
}

void GLMazeDialog::keyPressEvent(QKeyEvent *event)
{
    QDialog::keyPressEvent(event);
}
