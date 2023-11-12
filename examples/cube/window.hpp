#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "model.hpp"
#include "trackball.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  std::default_random_engine m_randomEngine;
  glm::ivec2 m_viewportSize{};
  Model m_model;

  struct Cube {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };
  std::array<Cube, 125> m_cubes;

  float m_angle{0.0};
  bool reverse{false};
  bool rotation{false};
  bool m_animation{false};

  int m_trianglesToDraw{};
  int collision{0};
  float m_distance{0.14366};
  float m_distance_v{0.14366};

  TrackBall m_trackBall;
  float m_zoom{2.0};

  glm::mat4 m_modelMatrix{2.0f};
  glm::mat4 m_viewMatrix{2.0f};
  glm::mat4 m_projMatrix{2.0f};

  GLuint m_program{};

  void randomizeCube(Cube &cube);
};

#endif
