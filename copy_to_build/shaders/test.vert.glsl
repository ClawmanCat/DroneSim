#version 410

in vec2 position;

out vec2 frag_position;

void main() {
	gl_Position.xy = position;
     gl_Position.z = 0.0;
     gl_Position.z = 1.0;
}