#include "mandelbrot.hpp"

#include <cassert>
#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "fwd.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

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
static glm::mat4 GetRotationMatrix();

static glm::vec4 camera_pos(0.0f, 0.0f, 0.0f, 0.0f);
static glm::ivec3 camera_rotation(0, 0, 0);

static float scale  = 1.0f;
static float offset = 0.03f;
static float dim_ratio = 1.0f;
static const float multiplier = 1.05f;
static const int angle_velocity = 1;

static const int kNDimensions = 3;
static const int kNTrianles   = 2;

static const float fill_screan[] = { 1,  1, 0, 
                                     1, -1, 0, 
                                    -1,  1, 0,
                                    -1,  1, 0, 
                                    -1, -1, 0, 
                                     1, -1, 0};

// global ----------------------------------------------------------------------

// rotating x y z
// translate x y z
// translate w
void Mandelbrot::TellAboutControls() {
    std::cout << "rotation  x: +Q -W" << std::endl
              << "rotation  y: +A -S" << std::endl
              << "rotation  z: +Z -X" << std::endl
              << "translate x: +E -R" << std::endl
              << "translate y: +D -F" << std::endl
              << "translate z: +C -V" << std::endl
              << "translate w: +1 -2" << std::endl
              << "zoom       :  +  _" << std::endl
              << "To default :  0   " << std::endl
              << "Quit       : Esc  " << std::endl; 
}

GLFWwindow* Mandelbrot::SetUpAndGetWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Mandelbrot::kOpenGLMajorVarsion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Mandelbrot::kOpenGLMinorVarsion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE); //NOTE

    GLFWwindow* window = glfwCreateWindow(Mandelbrot::kWindowWidth, 
                                          Mandelbrot::kWindowHeight,
                                          Mandelbrot::kWindowTitle, 
                                          nullptr, 
                                          nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { assert(0); }

    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);
    glfwSetKeyCallback(window, KeyboardCallback);

    return window;
}

// Render ----------------------------------------------------------------------

Mandelbrot::Render::Render(GLFWwindow* window,
                           const char* vertex_shader_file_name,
                           const char* fragment_shader_file_name)
    : shader_(vertex_shader_file_name, fragment_shader_file_name) {
#if defined(OBJECTS_CTORDTOR_DUMP)
    std::cerr << "render constructor" << std::endl;
#endif // DEBUG

    // glEnable(GL_FRAMEBUFFER_SRGB); //NOTE

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

Mandelbrot::Render::~Render() {
#if defined(OBJECTS_CTORDTOR_DUMP)
    std::cerr << "render destructor" << std::endl;
#endif // DEBUG
    glDeleteBuffers(1, &vbo_); $ 
    glDeleteVertexArrays(1, &vao_); $

    fill_screan_ = nullptr;

    glfwTerminate();
}

void Mandelbrot::Render::RenderFrame() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); $ // set clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); $ // clear screen

    glUseProgram(shader_.GetShaderProgramId()); $

    int translate_id = glGetUniformLocation(shader_.GetShaderProgramId(),
                                            "translate_parameter"); $ 
    glUniform4f(translate_id, camera_pos.x, camera_pos.y, camera_pos.z, camera_pos.w); $

    int scale_id = glGetUniformLocation(shader_.GetShaderProgramId(), 
                                        "scale_parameter"); $       
    glUniform1f(scale_id, scale); $

    int dim_ratio_id = glGetUniformLocation(shader_.GetShaderProgramId(), 
                                            "dim_ratio"); $       
    glUniform1f(dim_ratio_id, dim_ratio); $

    glm::mat4 rotate_mat = GetRotationMatrix();
    int rotation_id = glGetUniformLocation(shader_.GetShaderProgramId(),
                                           "rotation"); $
    glUniformMatrix4fv(rotation_id, 1, GL_FALSE, glm::value_ptr(rotate_mat));

    glBindVertexArray(vao_); $ 
    glDrawArrays(GL_TRIANGLES, 0, kNDimensions * kNTrianles); $

    glfwSwapBuffers(window_);
    glfwPollEvents();
};

int Mandelbrot::Render::CheckWindowShouldClose() {
    return glfwWindowShouldClose(window_);
}

// static ----------------------------------------------------------------------

static glm::mat4 GetRotationMatrix() {
    glm::mat4 rotate_mat_x = glm::mat4(1.0f);
    glm::mat4 rotate_mat_y = glm::mat4(1.0f);
    glm::mat4 rotate_mat_z = glm::mat4(1.0f);

    rotate_mat_x = glm::rotate(rotate_mat_x, glm::radians((float)camera_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotate_mat_y = glm::rotate(rotate_mat_y, glm::radians((float)camera_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotate_mat_z = glm::rotate(rotate_mat_z, glm::radians((float)camera_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return rotate_mat_x * rotate_mat_y * rotate_mat_z;
}

// callbacks -------------------------------------------------------------------

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
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_0:
            camera_pos = glm::vec4(0.0f);
            camera_rotation = glm::ivec3(0, 0, 0);
            scale = 1.0f;
            offset = 0.03f;
            break;
        // rotation 
        case GLFW_KEY_Q:
            camera_rotation.x += angle_velocity;
            break;
        case GLFW_KEY_W:
            camera_rotation.x -= angle_velocity;
            break;
        case GLFW_KEY_A:
            camera_rotation.y += angle_velocity;
            break;
        case GLFW_KEY_S:
            camera_rotation.y -= angle_velocity;
            break;
        case GLFW_KEY_Z:
            camera_rotation.z += angle_velocity;
            break;
        case GLFW_KEY_X:
            camera_rotation.z -= angle_velocity;
            break;
        // translation
        case GLFW_KEY_E:
            camera_pos.x -= offset;
            break;
        case GLFW_KEY_R:
            camera_pos.x += offset;
            break;
        case GLFW_KEY_D:
            camera_pos.y -= offset;
            break;
        case GLFW_KEY_F:
            camera_pos.y += offset;
            break;
        case GLFW_KEY_C:
            camera_pos.z -= offset;
            break;
        case GLFW_KEY_V:
            camera_pos.z += offset;
            break;
        case GLFW_KEY_1:
            camera_pos.w -= offset;
            break;
        case GLFW_KEY_2:
            camera_pos.w += offset;
            break;
        // zoom
        case GLFW_KEY_EQUAL:
            scale /= multiplier;
            offset /= multiplier;
            break;
        case GLFW_KEY_MINUS:
            scale *= multiplier;
            offset *= multiplier;
            break;
        // other keys
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