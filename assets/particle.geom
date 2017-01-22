#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat3 mvp;
uniform float texSize; // 1.0 / num de texturas

in vec2 geom_vel[];
in vec4 geom_color[];
in float geom_size[];
in int geom_texIndex[];

out vec2 v_texCoord;
out vec4 v_color;

void vtx(vec2 d) {
    vec2 pos = gl_in[0].gl_Position.xy + d * geom_size[0];
    gl_Position = vec4(mvp*vec3(pos, 1.0), 1.0);
    v_texCoord.x = texSize*(float(geom_texIndex[0]) + (d.x+1.0)/2.0);
    v_texCoord.y = (d.y+1.0)/2.0;
    v_color = geom_color[0];
    EmitVertex();
}

void main() {
    vtx(vec2( 1, 1));
    vtx(vec2(-1, 1));
    vtx(vec2( 1,-1));
    vtx(vec2(-1,-1));
    EndPrimitive();
}
