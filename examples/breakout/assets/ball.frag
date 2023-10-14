#version 300 es

precision mediump float;

in vec4 fragColor;

out vec4 outColor;

void main() {
  vec2 coord = gl_PointCoord - vec2(0.5);  //from [0,1] to [-0.5,0.5]
  if(length(coord) > 0.5)                  //outside of circle radius?
      discard;
  outColor = fragColor;
}