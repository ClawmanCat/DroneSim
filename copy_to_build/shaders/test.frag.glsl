#version 410

in vec2 frag_position;
in vec2 frag_uv;

uniform sampler2D texture_sampler;

out vec4 color;

void main() {
     color = texture(texture_sampler, frag_uv);
}