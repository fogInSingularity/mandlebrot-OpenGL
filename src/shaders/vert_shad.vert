#version 330 core

layout (location = 0) in vec4 vert_pos;
out vec4 color;

uniform float scale_parameter;
uniform vec4 translate_parameter;
uniform float dim_ratio;
uniform mat4 rotation;

void main() {
    gl_Position = vec4(vert_pos.x, vert_pos.y, vert_pos.z, vert_pos.w);
    color = rotation
            * (vec4(vert_pos.x * dim_ratio, vert_pos.y, vert_pos.z, vert_pos.w) 
               * scale_parameter 
               + translate_parameter);
}