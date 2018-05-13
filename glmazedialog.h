#ifndef GLMAZEDIALOG_H
#define GLMAZEDIALOG_H

#include <QDialog>

class QKeyEvent;

namespace Ui {
class GLMazeDialog;
}

class GLMazeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GLMazeDialog(QWidget *parent = 0);
    ~GLMazeDialog();

public slots:
    void on_sliderAmbient_valueChanged(int value);
    void on_sliderDiffuse_valueChanged(int value);
    void on_sliderSpecular_valueChanged(int value);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::GLMazeDialog *ui;
};

#endif // GLMAZEDIALOG_H
