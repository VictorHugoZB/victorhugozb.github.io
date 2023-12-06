#include "camera.hpp"

#include "abcg.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix = glm::mat4(1.0f);
  auto const aspect{size.x / size.y};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 50.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::update(glm::vec3 position) {
  float hDistance = m_protagonistDistance * glm::cos(m_verticalAngle);
  float vDistance = m_protagonistDistance * glm::sin(m_verticalAngle);

  float offsetX = hDistance * glm::sin(m_protagonistAngle);
  float offsetZ = hDistance * glm::cos(m_protagonistAngle);

  m_eye.y = position.y + vDistance;
  m_eye.x = position.x - offsetX;
  m_eye.z = position.z - offsetZ;

  m_at = position + glm::vec3{0.0f, 0.5f, 0.0f};
  computeViewMatrix();
}

void Camera::zoom(float scroll) {
  if (scroll > 0 && m_protagonistDistance < 2.0f)
    return;
  if (scroll < 0 && m_protagonistDistance > 15.0f)
    return;
  m_protagonistDistance -= m_protagonistDistance * scroll * 0.05f;
}

void Camera::rotateAroundProtagonist(float angleVariation) {
  m_protagonistAngle += angleVariation * 0.2f;
}

void Camera::mousePress(glm::ivec2 const &position) {
  m_lastMousePosition = project(position);
  m_mouseTracking = true;
}

void Camera::mouseRelease() { m_mouseTracking = false; }

void Camera::resizeViewport(glm::ivec2 const &size) { m_viewportSize = size; }

glm::vec3 Camera::project(glm::vec2 const &position) const {
  // Convert from window coordinates to NDC
  auto projected{glm::vec3(
      2.0f * position.x / gsl::narrow<float>(m_viewportSize.x) - 1.0f,
      1.0f - 2.0f * position.y / gsl::narrow<float>(m_viewportSize.y), 0.0f)};

  if (auto const squaredLength{glm::length2(projected)};
      squaredLength >= 1.0f) {
    projected = glm::normalize(projected);
  } else {
    projected.z = std::sqrt(1.0f - squaredLength);
  }

  return projected;
}

void Camera::mouseMove(glm::ivec2 const &position) {
  if (!m_mouseTracking)
    return;

  auto const currentPosition{project(position)};
  auto const horizontalDifference{currentPosition.x - m_lastMousePosition.x};
  auto const verticalDifference{currentPosition.y - m_lastMousePosition.y};

  m_protagonistAngle += horizontalDifference * 0.8f;

  if (!(m_verticalAngle >= glm::radians(85.0f) && verticalDifference > 0) &&
      !(m_verticalAngle <= glm::radians(5.0f) && verticalDifference < 0)) {
    m_verticalAngle += verticalDifference * 0.8f;
  }

  m_lastMousePosition = currentPosition;
}