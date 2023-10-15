#include "block.hpp"
#include "gamedata.hpp"

#include <glm/fwd.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <vector>

const std::vector<std::string> level2Indestructables{{
    "41",
    "42",
    "45",
    "46",
    "13",
    "14",
}};

const std::vector<std::string> level3Indestructables{
    {"10", "11", "12", "15", "16", "17", "22", "25", "31", "32", "35", "36"}};

bool checkIsDestructable(int row, int position, Level level) {
  if (level == Level::Level1)
    return true;

  std::string blockStr = std::to_string(row) + std::to_string(position);

  if (level == Level::Level2) {
    return std::find(level2Indestructables.begin(), level2Indestructables.end(),
                     blockStr) == level2Indestructables.end();
  }

  if (level == Level::Level3) {
    return std::find(level3Indestructables.begin(), level3Indestructables.end(),
                     blockStr) == level3Indestructables.end();
  }

  return false;
}

bool isInYRange(Ball ball, Blocks::Block block) {
  float bottomEdgeY = block.m_translation.y - 0.30f * block.m_scale / 2 - 0.01;
  float topEdgeY = block.m_translation.y + 0.30f * block.m_scale / 2 + 0.01;

  return ball.m_translation.y >= bottomEdgeY &&
         ball.m_translation.y <= topEdgeY;
}

bool isInXRange(Ball ball, Blocks::Block block) {
  float leftEdgeX = block.m_translation.x - block.m_scale - 0.01;
  float rightEdgeX = block.m_translation.x + block.m_scale + 0.01;

  return ball.m_translation.x >= leftEdgeX &&
         ball.m_translation.x <= rightEdgeX;
}

void Blocks::create(GLuint program, Level level) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_blocks.clear();
  m_blocks.resize(8 * 5);

  int i = 0;

  for (auto &block : m_blocks) {
    int position = i % 8;
    int row = i / 8;
    block = makeBlock(0.122, row, position,
                      checkIsDestructable(row, position, level));
    i++;
  }
}

void Blocks::paint() {
  abcg::glUseProgram(m_program);

  for (auto const &block : m_blocks) {
    abcg::glBindVertexArray(block.m_VAO);

    abcg::glUniform4fv(m_colorLoc, 1, &block.m_color.r);
    abcg::glUniform1f(m_scaleLoc, block.m_scale);

    abcg::glUniform2fv(m_translationLoc, 1, &block.m_translation.x);

    abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Blocks::destroy() {
  for (auto &block : m_blocks) {
    abcg::glDeleteBuffers(1, &block.m_VBO);
    abcg::glDeleteBuffers(1, &block.m_EBO);
    abcg::glDeleteVertexArrays(1, &block.m_VAO);
  }
}

void Blocks::update(Ball &ball) {
  for (auto &block : m_blocks) {
    float leftEdgeX = block.m_translation.x - block.m_scale;
    float rightEdgeX = block.m_translation.x + block.m_scale;
    float bottomEdgeY = block.m_translation.y - 0.30f * block.m_scale;
    float topEdgeY = block.m_translation.y + 0.30f * block.m_scale;

    float ballX = ball.m_translation.x;
    float ballY = ball.m_translation.y;

    if (isInYRange(ball, block) && abs(ballX - leftEdgeX) <= 0.02f) {
      ball.m_velocity = glm::vec2{-abs(ball.m_velocity.x), ball.m_velocity.y};
      block.m_hit = true;
    }

    if (isInYRange(ball, block) && abs(ballX - rightEdgeX) <= 0.02f) {
      ball.m_velocity = glm::vec2{abs(ball.m_velocity.x), ball.m_velocity.y};
      block.m_hit = true;
    }

    if (isInXRange(ball, block) && abs(ballY - bottomEdgeY) <= 0.01f) {
      ball.m_velocity = glm::vec2{ball.m_velocity.x, -abs(ball.m_velocity.y)};
      block.m_hit = true;
    }

    if (isInXRange(ball, block) && abs(ballY - topEdgeY) <= 0.01f) {
      ball.m_velocity = glm::vec2{ball.m_velocity.x, abs(ball.m_velocity.y)};
      block.m_hit = true;
    }
  }

  m_blocks.remove_if(
      [](Blocks::Block const &a) { return a.m_hit && a.m_destructable; });
}

Blocks::Block Blocks::makeBlock(float scale, int row, int position,
                                bool destructable) {
  Block block;
  float yTranslation = 0.95f - row * (0.3f * scale + 0.04f);
  float xTranslation = -1.00f + (1.00f / 8.00f) * (2.00f * position + 1.00f);

  block.m_color = destructable ? glm::vec4(1) : glm::vec4(glm::vec3(0.5), 1);
  block.m_scale = scale;
  block.m_translation = glm::vec2{xTranslation, yTranslation};
  block.m_destructable = destructable;

  std::array positions{
      glm::vec2{-1.00f, +0.30f},
      glm::vec2{-1.00f, -0.30f},
      glm::vec2{+1.00f, -0.30f},
      glm::vec2{+1.00, +0.30f},
  };

  std::array const indices{0, 1, 2, 2, 3, 0};

  // Generate VBO
  abcg::glGenBuffers(1, &block.m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, block.m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &block.m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block.m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &block.m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(block.m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, block.m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block.m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return block;
}