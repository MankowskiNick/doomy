#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D pixels;

void main() {
    FragColor = texture(pixels, TexCoord);
}