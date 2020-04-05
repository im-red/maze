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

#include "util.h"

#include <QDebug>

#include <chrono>

using namespace std;

pair<int, int> makeOrderedPair(int a, int b)
{
    pair<int, int> result(a, b);
    if (a > b)
    {
        result.first = b;
        result.second = a;
    }
    return result;
}

int StopWatch::m_aliveCount = 0;

StopWatch::StopWatch(const string &tag)
    : m_stopped(false)
    , m_startTimeInNs(0)
    , m_tag(tag)
{
    m_startTimeInNs = currentNSecsSinceEpoch();
    m_aliveCount++;

    log(prefix() + " " + m_tag + " begin");
}

StopWatch::~StopWatch()
{
    stop();
}

void StopWatch::stop()
{
    if (m_stopped)
    {
        return;
    }
    m_stopped = true;

    int64_t stopTime = currentNSecsSinceEpoch();
    int64_t elapsed = stopTime - m_startTimeInNs;
    double elapsedInMSecs = elapsed / 1000000.0;

    log(prefix() + " " + m_tag + " const " + to_string(elapsedInMSecs) + " ms");

    m_aliveCount--;
}

string StopWatch::prefix()
{
    return string(static_cast<size_t>(m_aliveCount), '|');
}

void StopWatch::log(const string &s)
{
    qDebug() << s.c_str();
}

int64_t currentNSecsSinceEpoch()
{
    using std::chrono::duration_cast;
    using std::chrono::nanoseconds;
    using std::chrono::steady_clock;

    auto tp = steady_clock::now();
    auto d = tp.time_since_epoch();
    int64_t result = duration_cast<nanoseconds>(d).count();

    return result;
}

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = QOpenGLContext::currentContext()->functions()->glGetError()) != GL_NO_ERROR)
    {
        const char *error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        qDebug() << error << " | " << file << " (" << line << ")";
    }
    return errorCode;
}

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar *message, const void *)
{
    // ignore non-significant error/warning codes
    // if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    qDebug() << "---------------";
    qDebug() << "Debug message (" << id << "):" <<  message;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             qDebug() << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   qDebug() << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: qDebug() << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     qDebug() << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     qDebug() << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           qDebug() << "Source: Other"; break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               qDebug() << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: qDebug() << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  qDebug() << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         qDebug() << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         qDebug() << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              qDebug() << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          qDebug() << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           qDebug() << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               qDebug() << "Type: Other"; break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         qDebug() << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       qDebug() << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          qDebug() << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: qDebug() << "Severity: notification"; break;
    }
}
