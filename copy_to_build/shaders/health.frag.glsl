#version 410

flat in vec2 frag_color;
out vec4 color;

void main() {
     color = vec4(frag_color, 0.0f, 1.0f);
}