#version 330

in vec2 a_position;
in vec2 a_vel;
in vec4 a_color;
in float a_size;
in int a_texIndex;
out vec2 geom_vel;
out vec4 geom_color;
out float geom_size;
out int geom_texIndex;

void main () {
    gl_Position = vec4(a_position, 1.0, 1.0);
    geom_color = a_color;
    geom_size = a_size;
    geom_texIndex = a_texIndex;
    geom_vel = a_vel;
}
