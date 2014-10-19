#version 420

uniform sampler2D tex;

in vec2 v_texcoord;
in vec3 v_normal;

out vec4 color;

void main() {
    color = texture(tex, v_texcoord);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float light = dot(v_normal, lightDir);
    light = max(0.0, light);
    color.rgb *= light;
}
