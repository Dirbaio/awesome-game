//precision mediump float;

attribute vec2 a_position;

uniform mat4 mvp;

void main() {
    gl_Position = mvp*vec4(a_position, 0.0, 1.0);
}
