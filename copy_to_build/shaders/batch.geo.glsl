// Required for guaranteeing division by zero is infinity.
#version 410

in vec2 vert_position;
in float vert_rotation;

out float geo_rotation; 

uniform vec2 size;
uniform Sampler2D texture;


void main() {
	gl_Position  = vec4(vert_position, 0, 0);
	geo_rotation = vert_rotation;
}