#version 410

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;


in uint[1] geo_value;
in uint[1] geo_index;

out vec2 uv;


uniform float offset;
uniform float height;
uniform float limit;


void main() {
     float dx = 1.0 - ((geo_value[0] / limit) * 2.0);
     float dy = -1.0 + offset + (geo_index[0] * height);

     vec2 vertices[6] = vec2[6](
          vec2(-1.0, dy         ),
          vec2(-1.0, dy + height),
          vec2(dx,   dy         ),
          vec2(dx,   dy + height),
          vec2(1.0,  dy         ),
          vec2(1.0,  dy + height)
     );

     
     #pragma optionNV(unroll all)
     for (uint i = 0; i < 6; ++i) {
          gl_Position.xy = vertices[i];
          gl_Position.z = 0.0;
          gl_Position.w = 1.0;


          uv = vec2(float(i >> 1) * 0.5, float(i & 1));
          

          EmitVertex();
          if (i > 4) EndPrimitive();
     }
}