#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "gamedata.hpp"
#include "ground.hpp"
#include "protagonist.hpp"
#include "trackball.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};
  GLuint m_program_model{};
  TrackBall m_trackBallLight;

  Camera m_camera;

  Ground m_ground;
  GameData m_gameData;
  Protagonist m_protagonist;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
};

#endif