#version 410

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;


in uint[1] geo_value;
in uint[1] geo_index;

flat out vec2 frag_color;


uniform uint  count;     // The number of healthbars being rendered.
uniform float offset;    // Y-offset for the healthbars.
uniform float height;    // The height of a healthbar.
uniform float limit;     // The amount of health in a healthbar.


void main() {
     // OpenGL seems to deactivate limit for some reason. When adding it here it is still considered used.
     float w  = 2.0f / count;
     float dx = (w * float(geo_index[0])) - 1.0f;


     float dy[2] = float[2](
          offset,
          offset + ((float(geo_value[0]) / limit) * height)
     );


     #pragma optionNV(unroll all)
     for (uint i = 0; i < 2; ++i) {
          float ymax = (i == 0) ? dy[1] : offset + height;


          vec2 color = (i == 0)
               ? vec2(0.0, 1.0)
               : vec2(1.0, 0.0);


          vec2 corners[4] = vec2[4](
               vec2(dx,     dy[i]),
               vec2(dx,     ymax ),
               vec2(dx + w, dy[i]),
               vec2(dx + w, ymax )
          );


          #pragma optionNV(unroll all)
          for (uint j = 0; j < 4; ++j) {
               gl_Position.xy = corners[j];
               gl_Position.zw = vec2(0.0, 1.0);

               frag_color = color;

               EmitVertex();
          }


          EndPrimitive();
     }
}