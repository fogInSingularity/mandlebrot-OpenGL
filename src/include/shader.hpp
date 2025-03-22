#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Shader {
  public:
    Shader(const char* vertex_shader_file_name,
           const char* fragment_shader_file_name);
    ~Shader();
    GLint GetShaderProgramId() const;
  private:
    GLint shader_program_id_;
};

#endif // SHADER_HPP_
