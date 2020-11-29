uniform sampler2D heightmap;
uniform vec2 viewport;

void main(){
    float scale = 1.0;
    float bias = 0.25;
    vec2 uv = gl_FragCoord.xy/viewport;

    float h = 0.0;

    h = scale * ((texture2D(heightmap, uv).r) - bias);
    clamp(h, 0.0, 1.0);
    gl_FragColor = vec4(0.0, h, 0.0, 1.0);
}