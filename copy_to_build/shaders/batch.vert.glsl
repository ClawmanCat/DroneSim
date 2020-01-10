#version 410

in vec2  position;
in float rotation;
in uint  frame;

out uint  geo_valid;
out float geo_rotation;
out uint  geo_frame;


const float infinity = 1.0 / 0.0;


void main() {
     gl_Position.xy = position;
     gl_Position.z  = 0.0;
     gl_Position.w  = 1.0;

     geo_valid    = uint(position.x != infinity && position.y != infinity);
     geo_rotation = rotation;
     geo_frame    = frame;
}