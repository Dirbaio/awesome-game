#version 420

#define M_PI 3.1415926535897932384626433832795

uniform float t;
uniform vec2 resolution;

out vec4 color;

float lolsin(float x) {
    return sin(x)*0.3+0.3;
}

void main() {
    vec2 p = ((gl_FragCoord.xy - resolution/2.0) / min(resolution.x, resolution.y)) * 2.0;

    float angle = atan(p.y, p.x);
    angle += t;
    angle *= 6/M_PI;

    float rad = length(p);

    if(fract(angle) > 0.5)
        color = vec4(1.0, 0.5, 0.0, 1.0);
    else
        color = vec4(1.0, 1.0, 0.0, 1.0);

    color = mix(vec4(1.0, 0.5, 0.0, 1.0), color, 1-exp(-rad));
}
