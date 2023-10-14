#ifndef BALL_HPP_
#define BALL_HPP_

#include <list>

#include "abcgOpenGL.hpp"

#include "bar.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Ball {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void update(Bar &bar, GameViewport gameViewport, float deltaTime);

  bool m_dead{false};
  glm::vec2 m_translation{};
  glm::vec2 m_velocity{};
  float m_velocityFactor{1.5f};

  float m_pointSize{8.0f};
  glm::vec4 m_color{1};

private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_pointSizeLoc{};
  GLint m_colorLoc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
};

#endif