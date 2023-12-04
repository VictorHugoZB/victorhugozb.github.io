#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "protagonist.hpp"
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
  void mousePress(glm::ivec2 const &position);
  void mouseMove(glm::ivec2 const &position);
  void mouseRelease();
  void computeViewMatrix();
  void computeProjectionMatrix(glm::vec2 const &size);
  void resizeViewport(glm::ivec2 const &size);

  void update(Protagonist &protagonist);
  void zoom(float scroll);
  void rotateAroundProtagonist(float angleVariation);
  [[nodiscard]] glm::vec3 project(glm::vec2 const &mousePosition) const;

  glm::mat4 const &getViewMatrix() const { return m_viewMatrix; }
  glm::mat4 const &getProjMatrix() const { return m_projMatrix; }

private:
  glm::vec3 m_eye{0.0f, 2.5f, -3.0f}; // Camera position
  glm::vec3 m_at{0.0f, 0.0f, 0.0f};   // Look-at point
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};   // "up" direction

  // Matrix to change from world space to camera space
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;
  glm::ivec2 m_viewportSize{};

  bool m_mouseTracking{false};

  // angle around protagonist
  float m_protagonistAngle{0.0f};

  // distance between camera and protagonist
  float m_protagonistDistance{glm::distance(m_eye, {0.0f, 0.0f, 0.0f})};

  // angle between camera and protagonist
  float m_verticalAngle{
      glm::acos(glm::dot(glm::normalize(m_eye - glm::vec3{0.0f, 0.0f, 0.0f}),
                         glm::normalize(glm::vec3{0.0f, 0.0f, -3.0f} -
                                        glm::vec3{0.0f, 0.0f, 0.0f})))};

  glm::vec3 m_lastMousePosition{};
};

#endif