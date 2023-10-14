#ifndef BORDER_HPP_
#define BORDER_HPP_

#include <list>
#include <random>

#include "abcgOpenGL.hpp"

#include "gamedata.hpp"

class Border {
public:
  void create(GLuint program);
  void paint(GameData const &gameData);
  void update();
  void destroy();

  float m_scale{1.00f};
  glm::vec4 m_color{1};
  glm::vec2 m_translation{};

private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
};

#endif