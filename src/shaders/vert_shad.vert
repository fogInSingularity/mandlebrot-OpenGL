#version 330 core

layout (location = 0) in vec4 vert_pos;
out vec4 color;

uniform float scale_parameter;
uniform vec4 translate_parameter;
uniform float dim_ratio;

void main() {
    gl_Position = vec4(vert_pos.x, vert_pos.y, vert_pos.z, 1.0f);
    color = vec4(vert_pos.x * dim_ratio, vert_pos.y, vert_pos.z, vert_pos.w) 
            * scale_parameter 
            + translate_parameter;
}