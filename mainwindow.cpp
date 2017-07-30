#include "unionfind.h"
#include "mazedata.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QBitArray>
#include <QPainter>
#include <iostream>
#include <ctime>
#include <random>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->rowSpinBox->setMinimum(2);
    ui->colSpinBox->setMinimum(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_okButton_clicked()
{
    generate(ui->rowSpinBox->value(), ui->colSpinBox->value());
}

void MainWindow::on_saveButton_clicked()
{
    ui->mazeWidget->save();
}

void MainWindow::generate(int row, int column)
{

    MazeData data(row, column);

    UnionFind uf(row * column);

    int *set = (int *)malloc(row * column * sizeof(int));
    int endOfSet = row * column;

    for (int i = 0; i < row * column; i++)
    {
        set[i] = i;
    }

    while (0 != endOfSet)
    {
        static std::default_random_engine e(time(0));
        int r = e() % endOfSet;
        int n = set[r];
        QVector<int> m;

        // the two node to connect
        int a = -1;
        int b = -1;

        if (0 == n)
        {
            if (!uf.Connected(0, column))
            {
                m.push_back(column);
            }
            if (!uf.Connected(0, 1))
            {
                m.push_back(1);
            }
        }
        else if (column - 1 == n)
        {
            if (!uf.Connected(n, n + column))
            {
                m.push_back(n + column);
            }
            if (!uf.Connected(n, n - 1))
            {
                m.push_back(n - 1);
            }
        }
        else if (column * (row - 1) == n)
        {
            if (!uf.Connected(n, n - column))
            {
                m.push_back(n - column);
            }
            if (!uf.Connected(n, n + 1))
            {
                m.push_back(n + 1);
            }
        }
        else if (column * row - 1 == n)
        {
            if (!uf.Connected(n, n - column))
            {
                m.push_back(n - column);
            }
            if (!uf.Connected(n, n - 1))
            {
                m.push_back(n - 1);
            }
        }
        else if (n >= 1 && n <= column - 2)
        {
            if (!uf.Connected(n, n + column))
            {
                m.push_back(n + column);
            }
            if (!uf.Connected(n, n - 1))
            {
                m.push_back(n - 1);
            }
            if (!uf.Connected(n, n + 1))
            {
                m.push_back(n + 1);
            }
        }
        else if (n % column == 0)
        {
            if (!uf.Connected(n, n - column))
            {
                m.push_back(n - column);
            }
            if (!uf.Connected(n, n + column))
            {
                m.push_back(n + column);
            }
            if (!uf.Connected(n, n + 1))
            {
                m.push_back(n + 1);
            }
        }
        else if (n % column == column - 1)
        {
            if (!uf.Connected(n, n - column))
            {
                m.push_back(n - column);
            }
            if (!uf.Connected(n, n + column))
            {
                m.push_back(n + column);
            }
            if (!uf.Connected(n, n - 1))
            {
                m.push_back(n - 1);
            }
        }
        else if (n >= column * (row - 1) + 1 && n <= column * row - 2)
        {
            if (!uf.Connected(n, n - column))
            {
                m.push_back(n - column);
            }
            if (!uf.Connected(n, n - 1))
            {
                m.push_back(n - 1);
            }
            if (!uf.Connected(n, n + 1))
            {
                m.push_back(n + 1);
            }
        }
        else
        {
            if (!uf.Connected(n, n - column))
            {
                m.push_back(n - column);
            }
            if (!uf.Connected(n, n + column))
            {
                m.push_back(n + column);
            }
            if (!uf.Connected(n, n - 1))
            {
                m.push_back(n - 1);
            }
            if (!uf.Connected(n, n + 1))
            {
                m.push_back(n + 1);
            }
        }

        if (0 == m.size())
        {
            endOfSet--;
            set[r] = set[endOfSet];
        }
        else if (1 == m.size())
        {
            a = n;
            b = m.at(0);

            uf.Union(a, b);
            endOfSet--;
            set[r] = set[endOfSet];
        }
        else
        {
            int random = e() % m.size();
            a = n;
            b = m.at(random);

            uf.Union(a, b);
        }

        if (-1 != a && -1 != b)
        {
            int rowA = a / column;
            int columnA = a % column;
            int rowB = b / column;
            int columnB = b % column;

            if (rowA == rowB)
            {
                if (columnA > columnB)
                {

                    data.m_vData[2 * rowA + 1].setBit(columnA, false);
                }
                else
                {
                    data.m_vData[2 * rowA + 1].setBit(columnB, false);
                }
            }
            else
            {
                if (rowA > rowB)
                {
                    data.m_vData[2 * rowA].setBit(columnA, false);
                }
                else
                {
                    data.m_vData[2 * rowB].setBit(columnA, false);
                }
            }
        }
    }

    ui->mazeWidget->setData(data);
    ui->mazeWidget->update();
}
