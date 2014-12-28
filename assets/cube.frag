uniform sampler2D tex;

varying vec2 v_texcoord;
varying vec3 v_normal;

void main() {
	gl_FragColor = texture2D(tex, v_texcoord);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	float light = dot(v_normal, lightDir);
	light = max(0.0, light);
	gl_FragColor.rgb *= light;
}
