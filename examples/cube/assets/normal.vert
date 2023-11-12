#version 300 es

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

out vec4 fragColor;

void main() {
  mat4 MVP = projMatrix * viewMatrix * modelMatrix;
  gl_Position = MVP * vec4(inPosition, 1.0);
  vec3 N = inNormal;
  vec3 rubikColor = vec3(0.5, 0.5, 0.5);

  // fonte de cores: https://rgbcolorpicker.com/0-1
  if (N.y > 0.0 && N.x == 0.0 && N.z == 0.0) {
    // rgb01(0.49, 1, 0.949) face-ciano
    rubikColor = vec3(0.49, 1, 0.949);
  } else if (N.y < 0.0 && N.x == 0.0 && N.z == 0.0) {
    // rgb01(0.988, 0, 0.871) face-pink
    rubikColor = vec3(0.988, 0, 0.871);
  } else if (N.x > 0.0 && N.y == 0.0 && N.z == 0.0) {
    // rgb01(0.831, 1, 0) face-verde-neon
    rubikColor = vec3(0.831, 1, 0);
  } else if (N.x < 0.0 && N.y == 0.0 && N.z == 0.0) {
    // rgb01(0.996, 0.776, 0.157) face-amarelo-dourado
    rubikColor = vec3(0.996, 0.776, 0.157);
  } else if (N.z > 0.0 && N.x == 0.0 && N.y == 0.0) {
    // rgb01(1, 1, 1) face-branca
    rubikColor = vec3(1.0, 1.0, 1.0);
  } else if (N.z < 0.0 && N.x == 0.0 && N.y == 0.0) {
    // rgb01(0.71, 0.216, 0.216) face-rubro
    rubikColor = vec3(0.71, 0.216, 0.216);
  }
  fragColor = vec4(rubikColor, 1.0);
}
