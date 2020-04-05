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

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <utility>
#include <string>

std::pair<int, int> makeOrderedPair(int a, int b);
int64_t currentNSecsSinceEpoch();

// glCheckError_ && glDebugOutput is from https://learnopengl.com/In-Practice/Debugging
// it's licensed under the terms of the CC BY-NC 4.0
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar *message, const void *);

class StopWatch
{
public:
    explicit StopWatch(const std::string &tag);
    ~StopWatch();

    void stop();

private:
    static std::string prefix();
    static void log(const std::string &s);

private:
    static int m_aliveCount;
    bool m_stopped;
    int64_t m_startTimeInNs;
    std::string m_tag;
};
