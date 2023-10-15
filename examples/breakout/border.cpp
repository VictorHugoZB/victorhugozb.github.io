#include "border.hpp"
#include "window.hpp"

#include <cstdio>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Border::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_translation = glm::vec2{0, 0};

  std::array positions{
      glm::vec2{-1.00f, -1.00f},
      glm::vec2{-1.00f, +1.00f},
      glm::vec2{+1.00f, +1.00f},
      glm::vec2{+1.00, -1.00f},
  };

  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions,
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  abcg::glGenVertexArrays(1, &m_VAO);

  abcg::glBindVertexArray(m_VAO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Border::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing)
    return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawArrays(GL_LINE_STRIP, 0, 4);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Border::update() { return; }

void Border::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}