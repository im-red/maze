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
    if (ui->aRadioButton->isChecked())
    {
        generateA(ui->rowSpinBox->value(), ui->colSpinBox->value());
    }
    else
    {
        generateB(ui->rowSpinBox->value(), ui->colSpinBox->value());
    }
}

void MainWindow::on_saveButton_clicked()
{
    ui->mazeWidget->save();
}

void MainWindow::generateA(int row, int column)
{

    DeepFirstSearch dfs(column, row);
    AdjacencyList list = dfs.generate();

    ui->mazeWidget->setPath(list);
}

void MainWindow::generateB(int row, int column)
{
    MazeData data(row, column);

    UnionFind uf(row * column);

    int *set = new int[row * column];
    int endOfSet = row * column;
    for (int i = 0; i < row * column; i++)
    {
        set[i] = i;
    }

    int *remain = new int[row * column];
    for (int i = 0; i < row * column; i++)
    {
        if (0 == i)
        {
            remain[i] = 2;
        }
        else if (column - 1 == i)
        {
            remain[i] = 2;
        }
        else if (column * (row - 1) == i)
        {
            remain[i] = 2;
        }
        else if (column * row - 1 == i)
        {
            remain[i] = 2;
        }
        else if (i >= 1 && i <= column - 2)
        {
            remain[i] = 3;
        }
        else if (i % column == 0)
        {
            remain[i] = 3;
        }
        else if (i % column == column - 1)
        {
            remain[i] = 3;
        }
        else if (i >= column * (row - 1) + 1 && i <= column * row - 2)
        {
            remain[i] = 3;
        }
        else
        {
            remain[i] = 4;
        }

    }

    endOfSet = row * column;
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

        QVector<int> temp;
        QVector<int>::iterator iter = m.begin();
        while (m.end() != iter)
        {
            if (remain[*iter] <= 2)
            {
                temp.push_back(*iter);
            }
            iter++;
        }
        iter = temp.begin();
        while (temp.end() != iter)
        {
            m.removeOne(*iter);
            iter++;
        }

        if (0 == m.size() || 1 == m.size() || 2 == m.size())
        {
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
            remain[a]--;
            remain[b]--;

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

    endOfSet = row * column;
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

        QVector<int> temp;
        QVector<int>::iterator iter = m.begin();
        while (m.end() != iter)
        {
            if (remain[*iter] <= 1)
            {
                temp.push_back(*iter);
            }
            iter++;
        }
        iter = temp.begin();
        while (temp.end() != iter)
        {
            m.removeOne(*iter);
            iter++;
        }

        if (0 == m.size() || 1 == m.size())
        {
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
            remain[a]--;
            remain[b]--;

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

    endOfSet = row * column;
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
        else
        {
            int random = e() % m.size();
            a = n;
            b = m.at(random);

            uf.Union(a, b);
        }

        if (-1 != a && -1 != b)
        {
            remain[a]--;
            remain[b]--;

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

    ui->mazeWidget->setWall(data);
    ui->mazeWidget->update();

    qDebug() << "count: " << uf.Connection();

    int degree[4] = { 0, 0, 0, 0};
    for (int i = 0; i < row * column; i++)
    {
        if (remain[i] == 0)
        {
            degree[3]++;
        }
        else if (remain[i] == 1)
        {
            degree[2]++;
        }
        else if (remain[i] == 2)
        {
            degree[1]++;
        }
        else
        {
            degree[0]++;
        }
    }
    qDebug() << "B";
    qDebug() << "1: " << degree[0] << " " << degree[0] * 100.0 / (row * column) << "%";
    qDebug() << "2: " << degree[1] << " " << degree[1] * 100.0 / (row * column) << "%";
    qDebug() << "3: " << degree[2] << " " << degree[2] * 100.0 / (row * column) << "%";
    qDebug() << "4: " << degree[3] << " " << degree[3] * 100.0 / (row * column) << "%";

    delete [] set;
    delete [] remain;
}
