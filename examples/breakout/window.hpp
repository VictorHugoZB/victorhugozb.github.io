#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "ball.hpp"
#include "bar.hpp"
#include "border.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void paintBorder();

private:
  glm::ivec2 m_viewportSize{};
  GameViewport m_gameViewport{};

  GLuint m_program{};
  GLuint m_program_ball{};

  GameData m_gameData;

  Bar m_bar;
  Border m_border;
  Ball m_ball;

  abcg::Timer m_restartWaitTimer;

  ImFont *m_font{};

  GLuint m_VAOBorder{};
  GLuint m_VBOBorder{};

  void restart();
  void checkCollisions();
  void checkWinCondition();
};

#endif