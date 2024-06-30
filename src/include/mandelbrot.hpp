#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.hpp"

#define UNUSED __attribute__((unused))

namespace Mandelbrot {
    static const int kOpenGLMajorVarsion = 3;
    static const int kOpenGLMinorVarsion = 3;
    static const int kWindowWidth = 1024;
    static const int kWindowHeight = 1024;
    static const char *kWindowTitle = "Mandelbrot";

    void TellAboutControls();
    GLFWwindow *SetUpAndGetWindow();

    class Render {
      public:
        Render(GLFWwindow *window, const char *vertex_shader_file_name,
                const char *fragment_shader_file_name);
        ~Render();
        void RenderFrame();
        int CheckWindowShouldClose();
      private:
        GLFWwindow *window_;
        Shader shader_;
        const float *fill_screan_;
        GLuint vao_;
        GLuint vbo_;
    };
} // namespace Mandelbrot

#endif // MANDELBROT_HPP_