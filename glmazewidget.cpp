#include "glmazewidget.h"

#include <QOpenGLShaderProgram>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QResizeEvent>
#include <QQuaternion>
#include <QtMath>

#include <cassert>
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>

static GLfloat vertices[] =
{
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,      0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

GLMazeWidget::GLMazeWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_isMouseCaptured(true)
    , m_zoom(-12.0)
    , m_WPressed(false)
    , m_SPressed(false)
    , m_APressed(false)
    , m_DPressed(false)
    , m_moveSpeed(0.05f)
    , m_mouseSensitivity(0.05f)
    , m_pitch(0.0f)
    , m_yaw(-90.0f)
    , m_roll(0.0f)
    , m_projLoc(0)
    , m_viewLoc(0)
    , m_modelLoc(0)
    , m_lightPosLoc(0)
{
    startTimer(10);
    captureMouse();
}

GLMazeWidget::~GLMazeWidget()
{
    makeCurrent();
    glDeleteBuffers(1, &m_vbo);
    glDeleteProgram(m_shaderProgram);
    doneCurrent();
    QGuiApplication::restoreOverrideCursor();
}

void GLMazeWidget::setAmbientStrength(float f)
{
    m_ambientStrength = f;
}

void GLMazeWidget::setDiffuseStrength(float f)
{
    m_diffuseStrength = f;
}

void GLMazeWidget::setSpecularStrength(float f)
{
    m_specularStrength = f;
}

void GLMazeWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    initShaders();
    initUniform();
    initUniformLoc();
    setUniform();
    initTextures();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void GLMazeWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUseProgram(m_shaderProgram);

    adjustCamera();

    glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, m_model.constData());
    glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, m_view.constData());
    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, m_proj.constData());
    glUniform3f(m_viewPosLoc, m_cameraPos.x(), m_cameraPos.y(), m_cameraPos.z());

    glUniform1f(m_ambientStrengthLoc, m_ambientStrength);
    glUniform1f(m_diffuseStrengthLoc, m_diffuseStrength);
    glUniform1f(m_specularStrengthLoc, m_specularStrength);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLfloat) / 8);
    glBindVertexArray(0);
}

void GLMazeWidget::resizeGL(int width, int height)
{
    // clear the projection matrix
    m_proj.setToIdentity();

    // adjust the ratio of width and height
    m_proj.perspective(45.0f, GLfloat(width) / height, 0.01f, 100.0f);
}

void GLMazeWidget::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        switch (event->key())
        {
        case Qt::Key_W:
        {
            m_WPressed = true;
            break;
        }
        case Qt::Key_S:
        {
            m_SPressed = true;
            break;
        }
        case Qt::Key_A:
        {
            m_APressed = true;
            break;
        }
        case Qt::Key_D:
        {
            m_DPressed = true;
            break;
        }
        case Qt::Key_Space:
        {
            if (m_isMouseCaptured)
            {
                releaseMouse();
            }
            else
            {
                captureMouse();
            }
        }
        default:
        {
            break;
        }
        }
    }

    QOpenGLWidget::keyPressEvent(event);
}

void GLMazeWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        switch (event->key())
        {
        case Qt::Key_W:
        {
            m_WPressed = false;
            break;
        }
        case Qt::Key_S:
        {
            m_SPressed = false;
            break;
        }
        case Qt::Key_A:
        {
            m_APressed = false;
            break;
        }
        case Qt::Key_D:
        {
            m_DPressed = false;
            break;
        }
        default:
        {
            break;
        }
        }
    }

    QOpenGLWidget::keyReleaseEvent(event);
}

void GLMazeWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_isMouseCaptured)
    {
        captureMouse();
    }
    QOpenGLWidget::mousePressEvent(event);
}

void GLMazeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QOpenGLWidget::mouseMoveEvent(event);
}

void GLMazeWidget::wheelEvent(QWheelEvent *event)
{
    QOpenGLWidget::wheelEvent(event);
}

void GLMazeWidget::timerEvent(QTimerEvent *event)
{
    (void)event;
    refresh();
}

void GLMazeWidget::resizeEvent(QResizeEvent *event)
{
    if (m_isMouseCaptured)
    {
        adjustCenterPos();
        QCursor::setPos(m_centerPos);
    }
    QOpenGLWidget::resizeEvent(event);
}

void GLMazeWidget::refresh()
{
    handleMouse();
    handleKey();
}

void GLMazeWidget::handleMouse()
{
    if (!m_isMouseCaptured)
    {
        return;
    }
    GLfloat xoffset = QCursor::pos().x() - m_lastPos.x();
    GLfloat yoffset = QCursor::pos().y() - m_lastPos.y();

    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch -= yoffset;

    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }

    m_cameraFront = QQuaternion::fromEulerAngles(m_pitch, m_yaw, m_roll).vector().normalized();
    float x = cosf(qDegreesToRadians(m_pitch)) * cosf(qDegreesToRadians(m_yaw));
    float y = sinf(qDegreesToRadians(m_pitch));
    float z = cosf(qDegreesToRadians(m_pitch)) * sinf(qDegreesToRadians(m_yaw));
    m_cameraFront = QVector3D(x, y, z);
    m_lastPos = m_centerPos;
    QCursor::setPos(m_centerPos);
}

void GLMazeWidget::handleKey()
{
    if (m_WPressed)
    {
        QVector3D moveFront(m_cameraFront.x(), 0, m_cameraFront.z());
        moveFront.normalize();
        m_cameraPos += m_moveSpeed * moveFront;
    }
    if (m_SPressed)
    {
        QVector3D moveFront(m_cameraFront.x(), 0, m_cameraFront.z());
        moveFront.normalize();
        m_cameraPos -= m_moveSpeed * moveFront;
    }
    if (m_APressed)
    {
        m_cameraPos -= m_moveSpeed * QVector3D::crossProduct(m_cameraFront, m_cameraUp).normalized();
    }
    if (m_DPressed)
    {
        m_cameraPos += m_moveSpeed * QVector3D::crossProduct(m_cameraFront, m_cameraUp).normalized();
    }
    update();
}

void GLMazeWidget::captureMouse()
{
    m_isMouseCaptured = true;
    setMouseTracking(true);
    QGuiApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    adjustCenterPos();
    QCursor::setPos(m_centerPos);
}

void GLMazeWidget::releaseMouse()
{
    m_isMouseCaptured = false;
    setMouseTracking(false);
    QGuiApplication::restoreOverrideCursor();
}

void GLMazeWidget::adjustCenterPos()
{
    m_centerPos.setX(width() / 2);
    m_centerPos.setY(height() / 2);
    m_lastPos = m_centerPos;
}

void GLMazeWidget::rotateBy(float angle, QVector3D &vector)
{
    /*
     * matrix multiplication does not satisfy the exchange law.
     * A * B != B * A
     * the newest rotate matrix should be attached to the front.
     */
    QMatrix4x4 temp = m_rotate;
    m_rotate.setToIdentity();
    m_rotate.rotate(angle, vector);
    m_rotate *= temp;

    // call the paintGL()
    update();
}

float GLMazeWidget::z(float x, float y)
{
    // trackball
    const float r = 1.0;
    if ((x * x + y * y) <= (r * r / 2)) {
        return sqrtf(r * r - (x * x + y * y));
    } else {
        return (r * r / 2 / sqrtf(x * x + y * y));
    }
}

void GLMazeWidget::initShaders()
{
    compileShader(m_vertexShader, ":/shader/vertex.glsl", GL_VERTEX_SHADER);
    compileShader(m_fragmentShader, ":/shader/fragment.glsl", GL_FRAGMENT_SHADER);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(m_shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        assert(false);
    }

    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
}

void GLMazeWidget::initUniformLoc()
{
    m_modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    m_viewLoc = glGetUniformLocation(m_shaderProgram, "view");
    m_projLoc = glGetUniformLocation(m_shaderProgram, "projection");
    m_lightPosLoc = glGetUniformLocation(m_shaderProgram, "lightPos");
    m_lightColorLoc = glGetUniformLocation(m_shaderProgram, "lightColor");
    m_ambientStrengthLoc = glGetUniformLocation(m_shaderProgram, "ambientStrength");
    m_diffuseStrengthLoc = glGetUniformLocation(m_shaderProgram, "diffuseStrength");
    m_specularStrengthLoc = glGetUniformLocation(m_shaderProgram, "specularStrength");
    m_viewPosLoc = glGetUniformLocation(m_shaderProgram, "viewPos");
}

void GLMazeWidget::initTextures()
{
    loadTexture(m_texture, ":/texture/cube.png", 0);
    bindTexture(0, "ourTexture1");
}

void GLMazeWidget::initUniform()
{
    m_cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    m_cameraFront = QVector3D(0.0f, -1.0f, -1.0f);
    m_cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    m_lightPos = QVector3D(0.0f, 0.0f, 5.0f);
    m_lightColor = QVector3D(1.0f, 1.0f, 1.0f);

    // the follow three uniform is set when slider change
    //m_ambientStrength = 0.5f;
    //m_diffuseStrength = 0.1f;
    //m_specularStrength = 0.5f;
}

void GLMazeWidget::setUniform()
{
    glUseProgram(m_shaderProgram);
    glUniform3f(m_lightPosLoc, m_lightPos.x(), m_lightPos.y(), m_lightPos.z());
    glUniform3f(m_lightColorLoc, m_lightColor.x(), m_lightColor.y(), m_lightColor.z());
}

void GLMazeWidget::adjustCamera()
{
    m_model.setToIdentity();
    m_model *= m_rotate;
    m_view.setToIdentity();
    //QVector3D cameraRight = QVector3D::crossProduct(m_cameraFront, QVector3D(0.0f, 1.0f, 0.0f));
    //m_cameraUp = QVector3D::crossProduct(cameraRight, m_cameraFront).normalized();
    m_view.lookAt(m_cameraPos, m_cameraFront + m_cameraPos, m_cameraUp);
}

void GLMazeWidget::loadTexture(GLuint &texture, const std::string &filepath, const GLint index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    QImage image(QString::fromStdString(filepath));
    QImage glImage = image.convertToFormat(QImage::Format_RGBA8888);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.constBits());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLMazeWidget::bindTexture(GLint index, const std::string &uniformName)
{
    glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName.c_str()), index);
}

void GLMazeWidget::compileShader(GLuint &shader, const std::string &filepath, const GLenum type)
{
    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        assert(false);
    }
    QTextStream stream(&file);
    std::string s = stream.readAll().toStdString();
    const char *str = s.c_str();
    file.close();

    shader = glCreateShader(type);

    glShaderSource(shader, 1, &str, nullptr);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        assert(false);
    }
}
