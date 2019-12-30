#version 410

in vec2 frag_position;
uniform float t;

out vec3 color;

void main() {
     vec2 fp = frag_position;

     color = vec3(sin(fp.x * t), cos(fp.y *t), -sin((fp.x + fp.y) / 2.0 * t));
}