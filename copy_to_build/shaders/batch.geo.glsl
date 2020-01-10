#version 410

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;


in uint[1]  geo_valid;
in float[1] geo_rotation;
in uint[1]  geo_frame;

out vec2 uv;


uniform vec2 window;
uniform vec2 size;
uniform uint fCount;


const vec2 corners[4] = vec2[4](
     vec2(0.0, 0.0),
     vec2(0.0, 1.0),
     vec2(1.0, 0.0),
     vec2(1.0, 1.0)
);


void main() {
     if (geo_valid[0] == 0) return;


     #pragma optionNV(unroll all)
     for (uint i = 0; i < 4; ++i) {
          gl_Position.xy = 2.0 * window * (gl_in[0].gl_Position.xy + (corners[i] * size) - 0.5 * size) - vec2(1.0, 1.0);
          gl_Position.z  = 0.0;
          gl_Position.w  = 1.0;


          float w = 1.0 / float(fCount);
          uv.x = (geo_frame[0] * w) + (corners[i].x * w);
          uv.y = -corners[i].y;


          EmitVertex();
          if (i > 2) EndPrimitive();
     }
}