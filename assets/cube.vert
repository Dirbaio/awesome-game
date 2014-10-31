attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec2 v_texcoord;
varying vec3 v_normal;

uniform mat4 mvp;
uniform mat3 norm;

void main() {
    v_texcoord = a_texCoord;
    v_normal = norm*a_normal;

    gl_Position = mvp*vec4(a_position, 1.0);
}
