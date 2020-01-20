#version 410

in vec2 position;
in uint frame;

out uint geo_frame;


void main() {
     gl_Position.xy = position;
     gl_Position.z  = 0.0;
     gl_Position.w  = 1.0;

     geo_frame = frame;
}