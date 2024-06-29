#include <cassert>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "mandelbrot.hpp"

int main(const int argc, const char** argv) {
    GLFWwindow* window = Mandelbort::SetUpAndGetWindow();

    Mandelbort::Render render(window, "src/shaders/vert_shad.vert",
                             "src/shaders/frag_shad.frag");

    while (!render.CheckWindowShouldClose()) {
        render.RenderFrame();
    }

    return 0;
}