#version 330 core
out vec4 FragColor;
uniform vec3 segColor;
void main() {
    FragColor = vec4(segColor, 1.0);
}