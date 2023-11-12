#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"
#include "protagonist.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
  void computeViewMatrix();
  void computeProjectionMatrix(glm::vec2 const &size);
  void mouseMove(glm::ivec2 const &position);
  void mousePress(glm::ivec2 const &position);
  void mouseRelease(glm::ivec2 const &position);

  [[nodiscard]] glm::mat4 getRotation() const;

  void pan(glm::ivec2 const &position, glm::vec3 protagonistPosition);

  void update(GameData const &gameData, float deltaTime,
              Protagonist protagonist);

  glm::vec3 m_eye{0.0f, 1.5f, -2.5f}; // Camera position
  glm::vec3 m_at{0.0f, 0.5f, 0.0f};   // Look-at point
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};   // "up" direction

  glm::mat4 const &getViewMatrix() const { return m_viewMatrix; }
  glm::mat4 const &getProjMatrix() const { return m_projMatrix; }
  glm::ivec2 m_viewportSize{};

private:
  // Matrix to change from world space to camera space
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;

  bool m_mouseTracking{};

  glm::vec3 m_axis{1.0f};
  glm::mat4 m_rotation{1.0f};
  float m_velocity{2.0f};

  glm::vec3 m_lastPosition{};
  abcg::Timer m_lastTime{};

  [[nodiscard]] glm::vec3 project(glm::vec2 const &mousePosition) const;
};

#endif