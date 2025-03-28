#include "opengl_log.hpp"

#include <cassert>
#include <cstdio>

#include <glad/gl.h>

// debug -----------------------------------------------------------------------
void GLLogError(const char* file_name, size_t line) {
    assert(file_name != nullptr);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "! Opengl error: %x, %s:%lu\n", error, file_name, line);
    }
}

void GLLogShaderError(GLint shader_id) {
#if defined(GRAPHICS_DEBUG_SHADER)
    int success = 0;
    char log[512];

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success); $ 
    if (!success) {
        glGetShaderInfoLog(shader_id, sizeof(log), NULL, log); $ 
        fprintf(stderr, "# gl error: shader: %s\n", log); $
    }
#endif // GRAPHICS_DEBUG
}

void GLLogLinkError(GLint shader_program) {
#if defined(GRAPHICS_DEBUG_SHADER)
    int success = 0;
    char log[512];

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success); $ 
    if (!success) {
        glGetProgramInfoLog(shader_program, sizeof(log), NULL, log); $ 
        fprintf(stderr, "# gl error: shader program: %s\n", log); $
    }
#endif // GRAPHICS_DEBUG
}
