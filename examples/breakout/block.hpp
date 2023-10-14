#ifndef ASTEROIDS_HPP_
#define ASTEROIDS_HPP_

#include <list>
#include <random>

#include "abcgOpenGL.hpp"

#include "ball.hpp"
#include "gamedata.hpp"

class Blocks {
public:
  void create(GLuint program, Level level);
  void paint();
  void destroy();
  void update(Ball &ball);

  struct Block {
    GLuint m_VAO{};
    GLuint m_EBO{};
    GLuint m_VBO{};

    glm::vec4 m_color{1};
    float m_scale{};
    glm::vec2 m_translation{};
    bool m_hit{};
    bool m_destructable;
  };

  std::list<Block> m_blocks;

  Block makeBlock(float scale = 0.25f, int row = 1, int position = 1,
                  bool destructable = true);

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
};

#endif