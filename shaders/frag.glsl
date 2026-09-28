#version 330 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D tex0;
uniform int renderMode; // 0: planet, 1: orbit, 2: procedural Saturn rings
void main() {
    if (renderMode == 1) {
        FragColor = vec4(0.32, 0.36, 0.42, 1.0);
    } else if (renderMode == 2) {
        float radius = texCoord.x;
        if (radius > 0.67 && radius < 0.72) discard;
        float bands = 0.72 + 0.12 * sin(radius * 180.0) + 0.08 * sin(radius * 470.0);
        vec3 color = mix(vec3(0.46, 0.39, 0.29), vec3(0.88, 0.81, 0.65), radius);
        FragColor = vec4(color * bands, 1.0);
    } else {
        FragColor = texture(tex0, texCoord);
    }
}
