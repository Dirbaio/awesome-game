precision mediump float;

#define M_PI 3.1415926535897932384626433832795

uniform float t;
uniform vec2 resolution;

float lolsin(float x) {
    return sin(x)*0.3+0.3;
}

void main() {
    vec2 p = ((gl_FragCoord.xy - resolution/2.0) / min(resolution.x, resolution.y)) * 2.0;

    float angle = atan(p.y, p.x);
    angle += t;
    angle *= 6.0/M_PI;

    float rad = length(p);

    vec4 outColor;
    if(fract(angle) > 0.5)
        outColor = vec4(1.0, 0.5, 0.0, 1.0);
    else
        outColor = vec4(1.0, 1.0, 0.0, 1.0);

    gl_FragColor = mix(vec4(1.0, 0.5, 0.0, 1.0), outColor, 1.0-exp(-rad));
}
