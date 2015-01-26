//precision mediump float;

attribute vec2 a_position;

uniform vec2 u_position;
uniform vec2 u_size;

void main() {
    gl_Position = vec4(u_position + a_position*u_size, 0.0, 1.0);
}
