#ifndef GLMAZEWIDGET_H
#define GLMAZEWIDGET_H

#include "adjacencylist.h"
#include "solutionlist.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include <string>

class QMouseEvent;
class QTimerEvent;
class QWheelEvent;
class QKeyEvent;
class QResizeEvent;

class GLMazeWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    GLMazeWidget(QWidget *parent = nullptr);
    ~GLMazeWidget();

    void setAdjacencyList(AdjacencyList &list) { m_adjList = list; }
    void setSolutionList(SolutionList &solution) { m_solution = solution; }

    void setAmbientStrength(float f);
    void setDiffuseStrength(float f);
    void setSpecularStrength(float f);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void refresh();
    void handleMouse();
    void handleKey();
    void captureMouse();
    void releaseMouse();
    void adjustCenterPos();
    // it's used to calculate the rotate matrix
    void rotateBy(float angle, QVector3D &vector);

    // calculate the z coordinate of trackball
    float z(float x, float y);

    void initShaders();
    void initUniformLoc();
    void initTextures();
    void initUniform();
    void setUniform();
    void adjustCamera();
    void loadTexture(GLuint &texture, const std::string &filepath, const GLint index = 0);
    void bindTexture(GLint index, const std::string &uniformName);
    void compileShader(GLuint &shader, const std::string &filepath, const GLenum type);

    AdjacencyList m_adjList;
    SolutionList m_solution;

    QPoint m_lastPos;
    QPoint m_centerPos;
    float m_zoom;

    bool m_isMouseCaptured;

    bool m_WPressed;
    bool m_SPressed;
    bool m_APressed;
    bool m_DPressed;

    float m_moveSpeed;
    float m_mouseSensitivity;

    float m_pitch;
    float m_yaw;
    float m_roll;

    QVector3D m_cameraPos;
    QVector3D m_cameraFront;
    QVector3D m_cameraUp;
    QVector3D m_lightPos;
    QVector3D m_lightColor;
    float m_ambientStrength;
    float m_diffuseStrength;
    float m_specularStrength;

    int m_projLoc;
    int m_viewLoc;
    int m_modelLoc;
    int m_lightPosLoc;
    int m_lightColorLoc;
    int m_viewPosLoc;
    int m_ambientStrengthLoc;
    int m_diffuseStrengthLoc;
    int m_specularStrengthLoc;

    QMatrix4x4 m_proj;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model;
    QMatrix4x4 m_rotate;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
    GLuint m_shaderProgram;
    GLuint m_texture;
};

#endif // GLMAZEWIDGET_H
