//precision mediump float;
varying vec2 v_dirtTexCoord;
varying vec2 v_grassTexCoord;
uniform sampler2D groundTexture;
uniform sampler2D grassTexture;
void main() {
    vec4 a  = texture2D(groundTexture, v_dirtTexCoord);
    vec4 b = texture2D(grassTexture, vec2(v_grassTexCoord.x, clamp(v_grassTexCoord.y, 0.001, 0.99)));

    gl_FragColor = vec4(mix(a.xyz, b.xyz, b.w), 1.0);
}
