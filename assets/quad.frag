#version 420

uniform float t;
uniform vec2 resolution;

out vec4 color;

float lolsin(float x) {
    return sin(x)*0.3+0.3;
}

void main() {
    vec2 p = (gl_FragCoord.xy / resolution) * 2.0 - 1.0;
    color = vec4(lolsin(p.x+p.y*8.23+t*4.132),
                 lolsin(p.x*2.62+p.y*0.93+t*1.332),
                 lolsin(p.x*1.2+p.y*6.23+t*3.132),
                 1.0);
}
