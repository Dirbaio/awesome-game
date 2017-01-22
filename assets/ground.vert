//precision mediump float;

attribute vec3 a_position;

uniform mat3 mvp;
uniform mat3 m;

varying vec2 v_dirtTexCoord;
varying vec2 v_grassTexCoord;

void main() {
    v_dirtTexCoord = (m*vec3(a_position.xy, 1.0)).xy*0.05;
    v_grassTexCoord = vec2(v_dirtTexCoord.x, a_position.z*30.0);
    gl_Position = vec4(mvp*vec3(a_position.xy, 1.0), 1.0);
}
