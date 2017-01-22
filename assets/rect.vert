//precision mediump float;

attribute vec2 a_position;
uniform mat3 mvp;

void main() {
    gl_Position = vec4(mvp*vec3(a_position, 1.0), 1.0);
}
