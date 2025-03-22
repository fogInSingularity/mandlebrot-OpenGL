#include "shader.hpp"

#include <iostream>
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "opengl_log.hpp"

#define DEBUG_PRINT \
    std::cerr << "debug " << __FILE__ << ':' << __LINE__ << std::endl;

// static ----------------------------------------------------------------------

static std::string LoadShaderFromFile(const char* shader_file_name);
static GLint CompileShader(std::string shader_str, GLenum shader_type); 
static GLint CompileShaderProgram(const char* vertex_shader_file_name,
                                  const char* fragment_shader_file_name);

// Shader ----------------------------------------------------------------------

Shader::Shader(const char* vertex_shader_file_name,
               const char* fragment_shader_file_name) {                
    assert(vertex_shader_file_name != nullptr);
    assert(fragment_shader_file_name != nullptr);
#if defined (OBJECTS_CTORDTOR_DUMP)
    std::cerr << "shader constructor" << std::endl;
#endif // DEBUG

    shader_program_id_ = CompileShaderProgram(vertex_shader_file_name, 
                                              fragment_shader_file_name); 
}

Shader::~Shader() {
#if defined (OBJECTS_CTORDTOR_DUMP)
    std::cerr << "shader destructor" << std::endl;
#endif // DEBUG
    if (shader_program_id_ != 0) {
        glDeleteProgram(shader_program_id_); $
    }
    shader_program_id_ = 0;
}

GLint Shader::GetShaderProgramId() const {
    return shader_program_id_;
}

// static ----------------------------------------------------------------------

static GLint CompileShaderProgram(const char* vertex_shader_file_name,
                                  const char* fragment_shader_file_name) {
    assert(vertex_shader_file_name != nullptr);
    assert(fragment_shader_file_name != nullptr);

    std::string vert_str = LoadShaderFromFile(vertex_shader_file_name);   
    std::string frag_str = LoadShaderFromFile(fragment_shader_file_name);

    GLint vert_id = CompileShader(vert_str, GL_VERTEX_SHADER);
    GLint frag_id = CompileShader(frag_str, GL_FRAGMENT_SHADER);

    GLint shader_program_id = glCreateProgram(); $
    glAttachShader(shader_program_id, vert_id); $
    glAttachShader(shader_program_id, frag_id); $
    glLinkProgram(shader_program_id); $
    GLLogLinkError(shader_program_id);

    glDeleteShader(vert_id); $
    glDeleteShader(frag_id); $

    return shader_program_id;
}

static std::string LoadShaderFromFile(const char* shader_file_name) {
    assert(shader_file_name != nullptr);

    std::ifstream shader_file(shader_file_name);
    if (!shader_file) { assert(0 && "unable to open shader file"); } // FIXME

    std::ostringstream file_stream;
    file_stream << shader_file.rdbuf();
    shader_file.close();

    return file_stream.str();
}

static GLint CompileShader(std::string shader_str, GLenum shader_type) {
    GLint shader_id = glCreateShader(shader_type); $
    
    const char* shader_raw_str = shader_str.c_str();
    glShaderSource(shader_id, 1, &shader_raw_str, nullptr); $
    glCompileShader(shader_id); $
    GLLogShaderError(shader_id); $

    return shader_id;
}
