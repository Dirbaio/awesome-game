//precision mediump float;

attribute vec2 a_position;
uniform mat4 mvp;

varying vec2 v_texcoord;
void main() {
    v_texcoord = a_position * 0.5 + 0.5;
    gl_Position = mvp*vec4(a_position, 0.0, 1.0);
}
