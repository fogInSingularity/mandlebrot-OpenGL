#include "mandelbrot.hpp"

#include <iostream>
#include <assert.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "opengl_log.hpp"
#include "shader.hpp"

// static ----------------------------------------------------------------------

static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

static float pos_x = 0.0f;
static float pos_y = 0.0f;
static float scale = 1.0f;
static float offset = 0.03f;
static const float multiplier = 1.05f;

// global ----------------------------------------------------------------------

GLFWwindow* Mandelbort::SetUpRender() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Mandelbort::kOpenGLMajorVarsion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Mandelbort::kOpenGLMinorVarsion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Mandelbort::kWindowWidth, 
                                          Mandelbort::kWindowHeight, 
                                          Mandelbort::kWindowTitle, 
                                          nullptr, 
                                          nullptr);

    if (window == nullptr) {
        assert(0);
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        assert(0);
        glfwTerminate();
        return nullptr;
    }

    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);
    glfwSetKeyCallback(window, KeyboardCallback);

    return window;
}

Shader Mandelbort::CompileShaders() {
    return Shader("src/shaders/vert_shad.vert", "src/shaders/frag_shad.frag");
}

void Mandelbort::Render(Shader shader_prog) {
    const float fill_screan[] = { 1,  1, 0,
                                  1, -1, 0,
                                 -1,  1, 0,
                                 -1,  1, 0,
                                 -1, -1, 0,
                                  1, -1, 0};
    const int kNDimensions = 3;
    const int kNumberTrianles = 2;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); $
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); $

    GLuint vao = 0;
    GLuint vbo = 0;

    glGenVertexArrays(1, &vao); $
    glGenBuffers(1, &vbo); $

    glBindVertexArray(vao); $

    glBindBuffer(GL_ARRAY_BUFFER, vbo); $
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
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); $
    glBindVertexArray(0); $

    glUseProgram(shader_prog.GetShaderProgramId()); $

    int translate_id = glGetUniformLocation(shader_prog.GetShaderProgramId(), "translate_parametr"); $
    glUniform3f(translate_id, pos_x, pos_y, 0.0f);

    int scale_id = glGetUniformLocation(shader_prog.GetShaderProgramId(), "scale_parametr");
    glUniform1f(scale_id, scale);

    glBindVertexArray(vao); $

    glEnableVertexAttribArray(0); $
    glDrawArrays(GL_TRIANGLES, 0, kNumberTrianles * kNDimensions); $
    glDisableVertexAttribArray(0); $

    glBindBuffer(GL_ARRAY_BUFFER, 0); $
    glBindVertexArray(0); $

    glDeleteBuffers(1, &vbo); $
    glDeleteVertexArrays(1, &vao); $
}

// static ----------------------------------------------------------------------

static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
    assert(window != nullptr);

    glViewport(0, 0, width, height); $
}

static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    assert(window != nullptr);

    (void)scancode;
    (void)action;
    (void)mods;
    
    switch (key) {
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_UP:
            pos_y += offset;
            break;
        case GLFW_KEY_DOWN:
            pos_y -= offset;
            break;
        case GLFW_KEY_RIGHT:
            pos_x += offset;
            break;
        case GLFW_KEY_LEFT:
            pos_x -= offset;
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
        #if defined (DEBUG)
            std::cerr << "unknown key: " << key << std::endl;
        #endif // DEBUG
            break;
    }
}