#version 330 core
in vec4 color;
out vec4 frag_color;

#define kMaxIterInt 128
#define kMaxIterFloat (float(kMaxIterInt))
#define kMaxIterRev (1 / kMaxIterFloat)

void main() {
    int iter = 0;

    vec2 z = vec2(color.x, color.y);
    while ((dot(z,z) < 4.0f) && (iter <= kMaxIterInt)) {
        z = vec2(z.x * z.x - z.y * z.y + color.x, 
             2 * z.x * z.y + color.y);
        iter++;
    }

    float color_ratio = (iter * kMaxIterRev);

    frag_color = vec4(color_ratio, color_ratio, color_ratio, 0.0f);
}