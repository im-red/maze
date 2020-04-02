#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd h:mm:ss.zzz} [%{type}] (%{file}:%{line}) %{function} - %{message}");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
