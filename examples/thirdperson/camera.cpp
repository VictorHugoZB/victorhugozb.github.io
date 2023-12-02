#include "camera.hpp"

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gsl/gsl_util>

glm::mat4 Camera::getRotation() const {
  if (m_mouseTracking)
    return m_rotation;

  // Rotate by velocity when not tracking to simulate an inertia-free rotation
  auto const angle{m_velocity * gsl::narrow_cast<float>(m_lastTime.elapsed())};

  return glm::rotate(glm::mat4(1.0f), angle, m_axis) * m_rotation;
}

void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix = glm::mat4(1.0f);
  auto const aspect{size.x / size.y};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 5.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::pan(glm::ivec2 const &position, glm::vec3 protagonistPosition) {
  // if (!m_mouseTracking)
  //   return;

  // auto const currentPosition{project(position)};

  // // Rotation axis
  // m_axis = glm::cross(m_lastPosition, currentPosition);

  // // Rotation angle
  // auto const angle{glm::length(m_axis)};

  // m_axis = glm::normalize(m_axis);

  // // Concatenate rotation: R_old = R_new * R_old
  // m_rotation = glm::rotate(glm::mat4(1.0f), angle, m_axis) * m_rotation;

  // m_lastPosition = currentPosition;

  // glm::mat4 transform{1.0f};

  // printf("ROTATING\n");
  // // Rotate camera around its local y axis
  // transform = glm::translate(transform, protagonistPosition);
  // transform = m_rotation;
  // transform = glm::translate(transform, -protagonistPosition);

  // m_at = transform * glm::vec4(m_at, 1.0f);

  // computeViewMatrix();
}

void Camera::mousePress(glm::ivec2 const &position) {
  m_rotation = getRotation();
  m_mouseTracking = true;
  m_lastPosition = project(position);
  m_velocity = 0.0f;
}

void Camera::mouseRelease(glm::ivec2 const &position) {
  m_mouseTracking = false;
}

glm::vec3 Camera::project(glm::vec2 const &position) const {
  // Convert from window coordinates to NDC
  auto projected{glm::vec3(
      2.0f * position.x / gsl::narrow<float>(m_viewportSize.x) - 1.0f,
      1.0f - 2.0f * position.y / gsl::narrow<float>(m_viewportSize.y), 0.0f)};

  // Project to centered unit hemisphere
  if (auto const squaredLength{glm::length2(projected)};
      squaredLength >= 1.0f) {
    // Outside the sphere
    projected = glm::normalize(projected);
  } else {
    // Inside the sphere
    projected.z = std::sqrt(1.0f - squaredLength);
  }

  return projected;
}

void Camera::update(GameData const &gameData, float deltaTime,
                    Protagonist protagonist) {

  m_angleAroundPlayer -= 0.1f * deltaTime;

  auto const protagonistPosition = protagonist.m_position;

  m_at = {protagonistPosition.x, protagonistPosition.y + 0.5f,
          protagonistPosition.z};

  computeViewMatrix();
}