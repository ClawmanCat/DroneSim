#version 410

in vec2 position;
in vec2 uv;

out vec2 frag_position;
out vec2 frag_uv;

void main() {
	gl_Position.xy = position;
     gl_Position.z = 0.0;
     gl_Position.z = 1.0;

     frag_uv = vec2(uv.x, -uv.y);
}