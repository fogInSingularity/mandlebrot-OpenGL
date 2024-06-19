#version 330 core

layout (location = 0) in vec3 vert_pos;
out vec4 color;

uniform float scale_parametr;
uniform vec3 translate_parametr;

void main() {
    gl_Position = vec4(vert_pos.x, vert_pos.y, vert_pos.z, 1.0f);
    color =  vec4(scale_parametr * vert_pos.x + translate_parametr.x, scale_parametr * vert_pos.y + translate_parametr.y, 0.0f , 1.0);
}