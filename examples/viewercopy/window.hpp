#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <glm/fwd.hpp>
#include <vector>

#include "abcgOpenGL.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "model.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  Model m_protagonist;
  Model m_ground;
  Model m_sand;
  Model m_tree;

  GameData m_gameData;

  glm::vec3 m_position{0.0f, 1.8f, 0.0f};
  glm::vec3 m_direction{0.0f, 0.0f, 1.0f};
  float m_angle{glm::radians(-90.0f)};

  Camera m_camera;

  float lastX = 300, lastY = 300;
  float yaw = -90.0f;
  float pitch = 0.0f;
  float sensitivity = 0.1f;
  glm::ivec2 mousePosition;

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  GLuint m_program_obj{};

  bool control = false;
  void checkCollision();

  // Light and material properties
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  std::vector<glm::vec2> m_treePositions = {
      {14.480562469339105, 5.29158969663807},
      {16.247475641624234, -11.137630965215113},
      {-10.841776717923004, 16.062203951958793},
      {-2.073098739236727, 12.634698239545884},
      {-7.185101799813314, -11.281882727862618},
      {-14.246062019711623, -8.03147614921249},
      {8.916644181247662, -11.881781695654903},
      {19.8478816056302, 11.437263595912881},
      {4.355972666879939, -8.00584260250158},
      {17.691231483682557, 1.9907769799115194},
      {14.729035566621043, 13.557036001752003},
      {10.977620161605621, -17.286372150090052},
      {17.159930370764407, 4.591748154468512},
      {2.578629854915853, 18.34622369218554},
      {4.5936487717452295, -0.37803132837381526},
      {-15.870576005014447, 1.6869255378651289},
      {-18.81044106130064, 9.891844728965744},
      {11.488239491899861, -12.517984636785009},
      {8.43044913496606, -16.57790392750367},
      {-14.385077879006563, 5.937288179266655},
      {-17.499877995530213, 18.28223349514316},
      {13.310447803502996, 11.926228708899362},
      {-19.737333789058155, 13.264232149362016},
      {-17.27986447261938, -8.299205015772948},
      {-12.294257081010311, -10.52605496591168},
      {8.565071730558842, 15.192367644664905},
      {17.425697900782524, 6.305773982751795},
      {13.49555286594206, 0.8451174837593527},
      {-18.613041120701197, -5.150910271314011},
      {-9.141513434373355, -8.455504717403045},
      {-9.96217227293775, -6.638881631832438},
      {9.802634968905526, -4.642152955724317},
      {-8.494916962083128, 4.749212133375025},
      {-7.555775268005411, 6.040037779480095},
      {-7.021086326280059, -9.092109492621677},
      {6.832992982016748, 9.798083617445485}};
};
#endif