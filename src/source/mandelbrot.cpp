#include "mandelbrot.hpp"

#include <cassert>
#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"

#include "opengl_log.hpp"
#include "shader.hpp"
#include "debug_cfg.hpp"

#define DEBUG_PRINT \
  std::cerr << "debug " << __FILE__ << ':' << __LINE__ << std::endl;

// static ----------------------------------------------------------------------

static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
static void KeyboardCallback(GLFWwindow* window, 
                             int key, 
                             int scancode,
                             int action, 
                             int mods);

static glm::vec3 camera_pos(0.0f, 0.0f, 0.0f);
static float scale  = 1.0f;
static float offset = 0.03f;
static float dim_ratio = 1.0f;

static const float multiplier = 1.05f;
static const int kNDimensions = 3;
static const int kNTrianles   = 2;

static const float fill_screan[] = { 1,  1, 0, 
                                     1, -1, 0, 
                                    -1,  1, 0,
                                    -1,  1, 0, 
                                    -1, -1, 0, 
                                     1, -1, 0};

// global ----------------------------------------------------------------------

GLFWwindow* Mandelbort::SetUpAndGetWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Mandelbort::kOpenGLMajorVarsion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Mandelbort::kOpenGLMinorVarsion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Mandelbort::kWindowWidth, 
                                          Mandelbort::kWindowHeight,
                                          Mandelbort::kWindowTitle, 
                                          nullptr, 
                                          nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { assert(0); }

    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);
    glfwSetKeyCallback(window, KeyboardCallback);

    return window;
}

// Render ----------------------------------------------------------------------

Mandelbort::Render::Render(GLFWwindow* window,
                           const char* vertex_shader_file_name,
                           const char* fragment_shader_file_name)
    : shader_(vertex_shader_file_name, fragment_shader_file_name) {
#if defined(OBJECTS_CTORDTOR_DUMP)
    std::cerr << "render constructor" << std::endl;
#endif // DEBUG

    window_      = window;
    fill_screan_ = fill_screan;

    // gl set up
    vao_ = 0;
    vbo_ = 0;

    glGenVertexArrays(1, &vao_); $ // generate vao
    glGenBuffers(1, &vbo_); $ // generate vbo

    glBindVertexArray(vao_); $ // make vao curent context

    glBindBuffer(GL_ARRAY_BUFFER, vbo_); $ 
    glBufferData(GL_ARRAY_BUFFER, 
                 (GLsizeiptr)sizeof(fill_screan), 
                 fill_screan,
                 GL_STATIC_DRAW); $

    glVertexAttribPointer(0, 
                          kNDimensions, 
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(float) * kNDimensions, 
                          (void*)0); $ 
    glEnableVertexAttribArray(0); $

    glBindBuffer(GL_ARRAY_BUFFER, 0); $ 
    glBindVertexArray(0); $
};

Mandelbort::Render::~Render() {
#if defined(OBJECTS_CTORDTOR_DUMP)
    std::cerr << "render destructor" << std::endl;
#endif // DEBUG
    glDeleteBuffers(1, &vbo_); $ 
    glDeleteVertexArrays(1, &vao_); $

    fill_screan_ = nullptr;

    glfwTerminate();
}

void Mandelbort::Render::RenderFrame() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); $ // set clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); $ // clear screen

    glUseProgram(shader_.GetShaderProgramId()); $

    int translate_id = glGetUniformLocation(shader_.GetShaderProgramId(),
                                            "translate_parameter"); $ 
    glUniform4f(translate_id, camera_pos.x, camera_pos.y, camera_pos.z, 0.0f); $

    int scale_id = glGetUniformLocation(shader_.GetShaderProgramId(), 
                                        "scale_parameter"); $       
    glUniform1f(scale_id, scale); $

    int dim_ratio_id = glGetUniformLocation(shader_.GetShaderProgramId(), 
                                            "dim_ratio"); $       
    glUniform1f(dim_ratio_id, dim_ratio);

    glBindVertexArray(vao_); $ 
    glDrawArrays(GL_TRIANGLES, 0, kNDimensions * kNTrianles); $

    glfwSwapBuffers(window_);
    glfwPollEvents();
};

int Mandelbort::Render::CheckWindowShouldClose() {
    return glfwWindowShouldClose(window_);
}

// static ----------------------------------------------------------------------

static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
    assert(window != nullptr);

    dim_ratio = (float)width / (float)height ;
    glViewport(0, 0, width, height); $
}

static void KeyboardCallback(GLFWwindow* window, 
                             int key, 
                             int scancode,
                             int action, 
                             int mods) {
    assert(window != nullptr);

    (void)scancode;
    (void)action;
    (void)mods;

    switch (key) {
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    case GLFW_KEY_UP:
        camera_pos.y += offset;
        break;
    case GLFW_KEY_DOWN:
        camera_pos.y -= offset;
        break;
    case GLFW_KEY_RIGHT:
        camera_pos.x += offset;
        break;
    case GLFW_KEY_LEFT:
        camera_pos.x -= offset;
        break;
    case GLFW_KEY_EQUAL:
        scale /= multiplier;
        offset /= multiplier;
        break;
    case GLFW_KEY_MINUS:
        scale *= multiplier;
        offset *= multiplier;
        break;
    default:
    #if defined(KEY_PRESSES_DUMP)
        std::cerr << "unknown key: " << key << std::endl;
    #endif // DEBUG
      break;
    }
#if defined(KEY_PRESSES_DUMP)
    std::cerr << "------------------------" << std::endl
              << "scale: "     << scale     << std::endl
              << "offset: "    << offset    << std::endl
              << "dim_ratio: " << dim_ratio << std::endl
              << "------------------------" << std::endl;
#endif // DEBUG
}