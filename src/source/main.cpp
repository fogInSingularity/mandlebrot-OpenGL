#include "mandelbrot.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cassert>

int main(const int argc, const char** argv) {
    Mandelbrot::TellAboutControls();
    GLFWwindow* window = Mandelbrot::SetUpAndGetWindow();

    Mandelbrot::Render render(window, 
                              "src/shaders/vert_shad.vert",
                              "src/shaders/mandelbrot.frag");

    while (!render.CheckWindowShouldClose()) {
        render.RenderFrame();
    }

    return 0;
}