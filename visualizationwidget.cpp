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

#include "visualizationwidget.h"
#include "util.h"

using namespace std;

static const float X_START = -0.999f;
static const float Y_START = -0.999f;
static const float X_END = 1.0f;
static const float Y_END = 1.0f;
static const float POINT_SIZE = 0.0f;

VisualizationWidget::VisualizationWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_step(0)
    , m_fps(0)
    , m_playing(false)
    , m_timerId(-1)
    , m_curActionSeq(0)
    , m_gaType(AdjacencyList::BreakWall)
    , m_vbo(0)
    , m_negativeVbo(0)
{
    qDebug() << __FUNCTION__;
}

void VisualizationWidget::setAdjacencyList(const AdjacencyList &adjacencyList)
{
    m_adjacencyList = adjacencyList;
    m_gaActions = adjacencyList.generationActions();
    m_gaType = m_adjacencyList.generationActionType();
    m_curActionSeq = 0;
}

void VisualizationWidget::setStep(int step)
{
    assert(step != 0);
    m_step = step;
}

void VisualizationWidget::setFps(int fps)
{
    assert(fps != 0);
    m_fps = fps;
    updateTimer();
}

void VisualizationWidget::play()
{
    m_playing = true;
    updateTimer();
}

void VisualizationWidget::pause()
{
    m_playing = false;
    updateTimer();
}

void VisualizationWidget::nextFrame()
{
    assert(validAdjacencyList());

    m_curActionSeq = nextActionSeq();
    updateVertices(m_curActionSeq);
    update();
}

void VisualizationWidget::prevFrame()
{
    assert(validAdjacencyList());

    m_curActionSeq = prevActionSeq();
    updateVertices(m_curActionSeq);
    update();
}

void VisualizationWidget::reset()
{
    assert(validAdjacencyList());

    m_curActionSeq = 0;
    updateVertices(m_curActionSeq);
    updateTimer();
    update();
}

void VisualizationWidget::timerEvent(QTimerEvent *event)
{
    nextFrame();
    update();
}

void VisualizationWidget::initializeGL()
{
    qDebug() << __FUNCTION__;
    initializeOpenGLFunctions();

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);

    const char vertexShaderSource[] =
            "#version 330 core\n"
            "layout (location = 0) in vec2 vPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(vPos.x, vPos.y, 0.0, 1.0);\n"
            "}\n";
    const char fragmentShaderSource[] =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
            "}\n";
    const char negativefragmentShaderSource[] =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
            "}\n";

    m_program.reset(new QOpenGLShaderProgram);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    m_negativeProgram.reset(new QOpenGLShaderProgram);
    m_negativeProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_negativeProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, negativefragmentShaderSource);
    m_negativeProgram->link();

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_negativeVbo);
}

void VisualizationWidget::paintGL()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    QRect rect = calcViewport();
    glViewport(rect.x(), rect.y(), rect.width(), rect.height());

    m_program->bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, m_vertices.size() / 2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_program->release();

    m_negativeProgram->bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_negativeVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glBufferData(GL_ARRAY_BUFFER, m_negativeVertices.size() * sizeof(float), m_negativeVertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, m_negativeVertices.size() / 2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_negativeProgram->release();
}

void VisualizationWidget::updateTimer()
{
    if (m_timerId != -1)
    {
        killTimer(m_timerId);
        m_timerId = -1;
    }

    if (m_playing)
    {
        int interval = 1000 / m_fps;
        m_timerId = startTimer(interval);
    }
    else
    {
        // do nothing if paused
    }
}

bool VisualizationWidget::validAdjacencyList() const
{
    if (!m_adjacencyList.valid())
    {
        return false;
    }
    if (!m_gaActions.size())
    {
        return false;
    }
    return true;
}

int VisualizationWidget::nextActionSeq()
{
    int result = m_curActionSeq;
    if (result == m_gaActions.size())
    {
        result = 0;
    }
    else
    {
        result += m_step;
        if (result > m_gaActions.size())
        {
            result = m_gaActions.size();
        }
    }
    return result;
}

int VisualizationWidget::prevActionSeq()
{
    int result = m_curActionSeq;
    if (result == 0)
    {
        result = m_gaActions.size();
    }
    else
    {
        result -= m_step;
        if (result < 0)
        {
            result = 0;
        }
    }
    return result;
}

void VisualizationWidget::updateVertices(int actionSeq)
{
    m_vertices.clear();
    m_negativeVertices.clear();

    const int row = m_adjacencyList.row();
    const int column = m_adjacencyList.column();
    const auto type = m_adjacencyList.generationActionType();

    const float rowSpacing = (Y_END - Y_START) / row;
    const float columnSpacing = (X_END - X_START) / column;
    const float spacing = min(rowSpacing, columnSpacing);

    const float w = column * spacing;
    const float h = row * spacing;

    appendLine(X_START, Y_START, X_START + w, Y_START, m_vertices);
    appendLine(X_START, Y_START, X_START, Y_START + h, m_vertices);
    appendLine(X_START + w, Y_START + h, X_START + w, Y_START, m_vertices);
    appendLine(X_START + w, Y_START + h, X_START, Y_START + h, m_vertices);

    if (type == AdjacencyList::BreakWall)
    {
        for (int r = 0; r <= row; r++)
        {
            float y = r * spacing + Y_START;
            appendLine(X_START, y, X_START + w, y, m_vertices);
        }
        for (int c = 0; c <= column; c++)
        {
            float x = c * spacing + X_START;
            appendLine(x, Y_START, x, Y_START + h, m_vertices);
        }
        for (int i = 0; i < actionSeq; i++)
        {
            breakWall(m_gaActions[i].first, m_gaActions[i].second, spacing);
        }
    }
    else
    {
        for (int i = 0; i < actionSeq; i++)
        {
            buildWall(m_gaActions[i].first, m_gaActions[i].second, spacing);
        }
    }
}

void VisualizationWidget::breakWall(int i, int j, float spacing)
{
    QLineF l = wall(i, j, spacing);
    appendLine(l.x1(), l.y1(), l.x2(), l.y2(), m_negativeVertices);
}

void VisualizationWidget::buildWall(int i, int j, float spacing)
{
    QLineF l = wall(i, j, spacing);
    appendLine(l.x1(), l.y1(), l.x2(), l.y2(), m_vertices);
}

QLineF VisualizationWidget::wall(int i, int j, float spacing)
{
    const int column = m_adjacencyList.column();
    const int iRow = i / column;
    const int iColumn = i % column;

    float x0 = 0;
    float y0 = 0;
    float x1 = 0;
    float y1 = 0;

    if (i + 1 == j)
    {
        x0 = X_START + (iColumn + 1) * spacing;
        y0 = Y_START + iRow * spacing + POINT_SIZE;
        x1 = x0;
        y1 = Y_START + (iRow + 1) * spacing - POINT_SIZE;
    }
    else
    {
        x0 = X_START + iColumn * spacing + POINT_SIZE;
        y0 = Y_START + (iRow + 1) * spacing;
        x1 = X_START + (iColumn + 1) * spacing - POINT_SIZE;
        y1 = y0;
    }
    return QLineF(x0, y0, x1, y1);
}

void VisualizationWidget::appendVertex(float x, float y, vector<float> &vertices)
{
    vertices.push_back(x);
    vertices.push_back(y);
}

void VisualizationWidget::appendLine(float x0, float y0, float x1, float y1, vector<float> &vertices)
{
    appendVertex(x0, Y_END - (y0 - Y_START), vertices);
    appendVertex(x1, Y_END - (y1 - Y_START), vertices);
}

QRect VisualizationWidget::calcViewport()
{
    const int w = width();
    const int h = height();

    const int len = min(w, h);

    QRect result;
    result.setX(0);
    result.setY(0);
    result.setWidth(len);
    result.setHeight(len);

    if (w <= h)
    {
        result.moveTop((h - len) / 2);
    }
    else
    {
        result.moveLeft((w - len) / 2);
    }
    return result;
}
