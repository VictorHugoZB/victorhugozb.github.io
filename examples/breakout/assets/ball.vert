#version 300 es

layout(location = 0) in vec2 inPosition;

uniform vec2 translation;
uniform float pointSize;
uniform vec4 color;

out vec4 fragColor;

void main() {
  gl_PointSize = pointSize;
  gl_Position = vec4(inPosition.xy + translation, 0, 1);
  fragColor = color;
}