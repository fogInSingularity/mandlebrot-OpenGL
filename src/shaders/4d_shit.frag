#version 330 core

#define kMaxIter 1024.0f

in vec4 color;
out vec4 frag_color;

float sqr(float x) {
    return x * x;
}

void main() {
    frag_color = color;

    float iter = 0.0f;

    float cnst_x = color.x;
    float cnst_y = color.y;
    float cnst_z = color.z;
    float cnst_w = color.w;

    float a = color.x;
    float b = color.y;
    float c = color.z;
    float d = color.w;

    while ((sqr(a) + sqr(b) + sqr(c) + sqr(d) < 4.0f) && (iter <= kMaxIter)) {
        float a_tmp = sqr(a) - sqr(b) - sqr(c) - sqr(d) + cnst_x;
        float b_tmp = 2 * a * b + cnst_y;
        float c_tmp = 2 * a * c + cnst_z;
        float d_tmp = 2 * a * d + cnst_w;

        a = a_tmp;
        b = b_tmp;
        c = c_tmp;
        d = d_tmp;

        iter++;
    }

    frag_color.x = iter / kMaxIter;
    frag_color.y = iter / kMaxIter;
    frag_color.z = iter / kMaxIter;
    frag_color.w = iter / kMaxIter;
}