#ifndef PROTAGONIST_HPP_
#define PROTAGONIST_HPP_

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position;

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Protagonist {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void loadModelFromFile(std::string_view path);

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
};

#endif