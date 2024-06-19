#include <cassert>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "mandelbrot.hpp"

int main(const int argc, const char** argv) {
    GLFWwindow* window = Mandelbort::SetUpRender();
    if (window == nullptr) { assert(0 && "cant open window"); } //FIXME

    Shader shader_prog = Mandelbort::CompileShaders();

    while (!glfwWindowShouldClose(window)) {
        Mandelbort::Render(shader_prog);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}