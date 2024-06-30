#version 330 core
in vec4 color;
out vec4 frag_color;

#define kMaxIter 128.0f

void main() {
    frag_color = color;

// --------------------
    float x = 0;
    float y = 0;

    float iter = 0;

    float real = color.x;
    float imag = color.y;

    float x_mul = 0;
    float y_mul = 0;

    while ((x_mul + y_mul < 4.0f) && (iter <= kMaxIter)) {
        float x_temp = x_mul - y_mul + real;
        y = 2 * x * y + imag;
        x = x_temp;

        x_mul = x * x;
        y_mul = y * y;

        iter++;
    }
// --------------------

    frag_color.x = iter / kMaxIter;
    frag_color.y = iter / kMaxIter;
    frag_color.z = iter / kMaxIter;
}