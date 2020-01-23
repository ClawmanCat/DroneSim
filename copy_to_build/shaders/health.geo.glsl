#version 410

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;


in uint[1] geo_value;
in uint[1] geo_index;

out vec2 uv;


uniform uint  count;     // The number of healthbars being rendered.
uniform float offset;    // Y-offset for the healthbars.
uniform float height;    // The height of a healthbar.
uniform float limit;     // The amount of health in a healthbar.


void main() {
     float w   = 2.0 / float(count);
     float dx  = 1.0 - (w * geo_index[0]);
     float dy  = (float(geo_value[0]) / limit) * height;


     vec2 vertices[6] = vec2[6](
          vec2(dx,     offset),
          vec2(dx - w, offset),
          vec2(dx,     offset + dy),
          vec2(dx - w, offset + dy),
          vec2(dx,     offset + height),
          vec2(dx - w, offset + height)
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