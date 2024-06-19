#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.hpp"

#define UNUSED __attribute__((unused))

namespace Mandelbort {
    static const int kOpenGLMajorVarsion = 3;
    static const int kOpenGLMinorVarsion = 3;
    static const int kWindowWidth = 1024;
    static const int kWindowHeight = 1024;
    static const char* kWindowTitle = "Mandelbrot";

    GLFWwindow* SetUpRender();
    Shader CompileShaders();
    void Render(Shader shader_prog);
}

#endif // MANDELBROT_HPP_