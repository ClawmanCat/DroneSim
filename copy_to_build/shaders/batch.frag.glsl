#version 410

in vec2 uv;
out vec4 color;

uniform sampler2D sampler;



void main() {
     // UV is exactly at the corner of the pixel. Floating point errors may cause it to dip below,
     // so add a small offset.
     color = texture(sampler, uv + vec2(0.00001, 0.00001));
}