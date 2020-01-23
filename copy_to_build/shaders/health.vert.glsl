#version 410

in uint value;

out uint geo_value;
out uint geo_index;

void main() {
     geo_value = value;
     geo_index = gl_VertexID;

     gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
}