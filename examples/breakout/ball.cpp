#include "ball.hpp"
#include "gamedata.hpp"

#include <cmath>
#include <cstdio>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <numbers>

float mapIntervalToAngle(float initial, float final, float target) {
  float c = std::numbers::pi / 6.00f;
  float d = c * 5.00f;

  return c + ((d - c) / (final - initial)) * (target - initial);
}

void Ball::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  m_pointSizeLoc = abcg::glGetUniformLocation(m_program, "pointSize");

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  m_translation = glm::vec2(0);
  m_dead = false;
  m_velocity = glm::normalize(glm::vec2{0.80f, -0.3f}) * 1.00f;

  std::array positions{
      glm::vec2{0.00, +0.00f},
  };

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE,
                              sizeof(glm::vec3) * 2, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Ball::paint() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);
  abcg::glUniform4f(m_colorLoc, 1, 1, 1, 1);
  abcg::glUniform1f(m_pointSizeLoc, m_pointSize);

  abcg::glUniform2f(m_translationLoc, m_translation.x, m_translation.y);

  abcg::glDrawArrays(GL_POINTS, 0, 1);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Ball::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Ball::update(Bar &bar, GameViewport gameViewport, float deltaTime) {
  if (m_translation.x + m_pointSize / (2 * gameViewport.x) >= 1.0f) {
    m_velocity = glm::vec2{-abs(m_velocity.x), m_velocity.y};
  }

  if (m_translation.x - m_pointSize / (2 * gameViewport.x) <= -1.0f) {
    m_velocity = glm::vec2{abs(m_velocity.x), m_velocity.y};
  }

  if (m_translation.y + m_pointSize / (2 * gameViewport.x) >= 1.0f) {
    m_velocity = glm::vec2{m_velocity.x, -abs(m_velocity.y)};
  }

  if (m_translation.y - m_pointSize / (2 * gameViewport.x) <= -1.0f) {
    m_dead = true;
  }

  if (m_velocity.y < 0 &&
      m_translation.y - m_pointSize / (2 * gameViewport.x) >=
          bar.m_translation.y + (0.03f * bar.m_scale) - 0.01 &&
      m_translation.y - m_pointSize / (2 * gameViewport.x) <=
          bar.m_translation.y + (0.03f * bar.m_scale) + 0.01 &&
      m_translation.x + m_pointSize / (2 * gameViewport.x) >=
          bar.m_translation.x - bar.m_scale &&
      m_translation.x - m_pointSize / (2 * gameViewport.x) <=
          bar.m_translation.x + bar.m_scale) {

    float initial = bar.m_translation.x - bar.m_scale / 2;
    float final = bar.m_translation.x + bar.m_scale / 2;
    float target = std::min(std::max(m_translation.x, initial), final);
    float angle = mapIntervalToAngle(initial, final, target);
    m_velocity = glm::normalize(glm::vec2{-cos(angle), abs(sin(angle))}) *
                 m_velocityFactor;
  }

  m_translation += m_velocity * deltaTime;
}