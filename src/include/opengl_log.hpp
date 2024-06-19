#ifndef OPENGL_LOG_HPP_
#define OPENGL_LOG_HPP_

#include <stddef.h>
#include <stdio.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define GRAPHICS_DEBUG_SHADER 1
#define GRAPHICS_DEBUG_CALLS 1

// debug 
#ifdef GRAPHICS_DEBUG_CALLS
    #define $ GLLogError(__FILE__, __LINE__); 
#else
    #define $ ;
#endif // GRAPHICS_DEBUG_CALLS

void GLLogError(const char* file_name, size_t line);
void GLLogShaderError(GLint shader_id);
void GLLogLinkError(GLint shader_program);

#endif // OPENGL_LOG_HPP_