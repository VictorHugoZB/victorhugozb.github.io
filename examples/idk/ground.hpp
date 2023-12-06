#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Ground {
public:
  void create(GLuint program);
  void paint(glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 rotation);
  void destroy();
  void loadModelFromFile(std::string_view path);
  void loadDiffuseTexture(std::string_view path);

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{1.0f};
  glm::vec4 m_Kd{1.0f};
  glm::vec4 m_Ks{0.0f};

  float m_shininess{};
  GLuint m_diffuseTexture{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{3};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
  GLint m_lightDirLoc{};
  GLint m_IaLoc{};
  GLint m_IdLoc{};
  GLint m_IsLoc{};
  GLint m_KaLoc{};
  GLint m_KdLoc{};
  GLint m_KsLoc{};
  GLint m_shininessLoc{};
  GLint m_diffuseTextureLoc{};
  GLint m_mappingModeLoc{};
  GLint m_normalMatrixLoc{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  void loadModel(std::string_view path);
  void computeNormals();
};

#endif