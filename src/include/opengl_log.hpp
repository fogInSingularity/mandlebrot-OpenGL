#ifndef OPENGL_LOG_HPP_
#define OPENGL_LOG_HPP_

#include <stddef.h>
#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

// debug
#if defined(GRAPHICS_DEBUG_CALLS)
    #define $ GLLogError(__FILE__, __LINE__);
#else
    #define $ ;
#endif // GRAPHICS_DEBUG_CALLS

void GLLogError(const char* file_name, size_t line);
void GLLogShaderError(GLint shader_id);
void GLLogLinkError(GLint shader_program);

#endif // OPENGL_LOG_HPP_
