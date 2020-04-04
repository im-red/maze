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

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QEvent>

#include <vector>
#include <memory>

class VisualizationWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    explicit VisualizationWidget(QWidget *parent = nullptr);

    void setAdjacencyList(const AdjacencyList &adjacencyList);
    void setStep(int step);
    void setFps(int fps);

    void play();
    void pause();
    void nextFrame();
    void prevFrame();
    void reset();

protected:
    void timerEvent(QTimerEvent *event) override;

    void initializeGL() override;
    void paintGL() override;

private:
    void updateTimer();
    bool validAdjacencyList() const;
    int nextActionSeq();
    int prevActionSeq();
    void updateVertices(int actionSeq);

    void breakWall(int i, int j, float spacing);
    void buildWall(int i, int j, float spacing);
    QLineF wall(int i, int j, float spacing);

    void appendVertex(float x, float y, std::vector<float> &vertices);
    void appendLine(float x0, float y0, float x1, float y1, std::vector<float> &vertices);

    QRect calcViewport();

private:
    AdjacencyList m_adjacencyList;
    int m_step;
    int m_fps;
    bool m_playing;
    int m_timerId;
    int m_curActionSeq;

    std::vector<std::pair<int, int>> m_gaActions;
    AdjacencyList::GenerationActionType m_gaType;
    std::vector<float> m_vertices;
    std::vector<float> m_negativeVertices;

    unsigned m_vbo;
    unsigned m_negativeVbo;
    std::unique_ptr<QOpenGLShaderProgram> m_program;
    std::unique_ptr<QOpenGLShaderProgram> m_negativeProgram;
};

