#include "window.hpp"

#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Right));
  }

  if (event.type == SDL_MOUSEWHEEL) {
    m_camera.zoom(event.wheel.y);
  }

  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_MOUSEMOTION) {
    m_camera.mouseMove(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN &&
      event.button.button == SDL_BUTTON_LEFT) {
    m_camera.mousePress(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONUP &&
      event.button.button == SDL_BUTTON_LEFT) {
    m_camera.mouseRelease();
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  m_program_obj =
      abcg::createOpenGLProgram({{.source = assetsPath + "shaders/texture.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "shaders/texture.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_protagonist.loadDiffuseTexture(assetsPath +
                                   "maps/Minecraft_steve_skin.jpg");
  m_protagonist.loadObj(assetsPath + "minecraft_steve.obj", false);
  m_protagonist.setupVAO(m_program_obj);

  m_Ka = m_protagonist.getKa();
  m_Kd = m_protagonist.getKd();
  m_Ks = m_protagonist.getKs();
  m_shininess = m_protagonist.getShininess();

  m_ground.loadDiffuseTexture(assetsPath + "maps/Grass_Block_TEX.png");
  m_ground.loadObj(assetsPath + "Grass_Block.obj");
  m_ground.setupVAO(m_program_obj);

  m_sand.loadDiffuseTexture(assetsPath + "maps/sand.png");
  m_sand.loadObj(assetsPath + "sand.obj");
  m_sand.setupVAO(m_program_obj);

  m_tree.loadDiffuseTexture(assetsPath + "maps/Mineways2Skfb-RGBA.png");
  m_tree.loadObj(assetsPath + "Mineways2Skfb.obj");
  m_tree.setupVAO(m_program_obj);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  if (m_gameData.m_input[static_cast<size_t>(Input::Left)]) {
    m_angle = glm::wrapAngle(m_angle + glm::radians(135.0f) * deltaTime);
    m_direction = glm::vec3(-glm::cos(m_angle + std::numbers::pi), 0.0f,
                            glm::sin(m_angle + std::numbers::pi));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::Right)]) {
    m_angle = glm::wrapAngle(m_angle - glm::radians(135.0f) * deltaTime);
    m_direction = glm::vec3(-glm::cos(m_angle + std::numbers::pi), 0.0f,
                            glm::sin(m_angle + std::numbers::pi));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::Up)])
    m_position += m_direction * deltaTime * 4.5f;
  if (m_gameData.m_input[static_cast<size_t>(Input::Down)])
    m_position -= m_direction * deltaTime * 4.5f;

  m_camera.update(m_position);
}

void Window::onPaint() {

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program_obj);

  // Get location of uniform variables
  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(m_program_obj, "viewMatrix")};
  auto const projMatrixLoc{
      abcg::glGetUniformLocation(m_program_obj, "projMatrix")};
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program_obj, "modelMatrix")};
  auto const normalMatrixLoc{
      abcg::glGetUniformLocation(m_program_obj, "normalMatrix")};
  auto const lightDirLoc{
      abcg::glGetUniformLocation(m_program_obj, "lightDirWorldSpace")};
  auto const shininessLoc{
      abcg::glGetUniformLocation(m_program_obj, "shininess")};
  auto const IaLoc{abcg::glGetUniformLocation(m_program_obj, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(m_program_obj, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(m_program_obj, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(m_program_obj, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(m_program_obj, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(m_program_obj, "Ks")};
  auto const diffuseTexLoc{
      abcg::glGetUniformLocation(m_program_obj, "diffuseTex")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);

  auto const lightDirRotated{glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)};

  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables for the current model
  glm::mat4 modelMatrixProt{1.0f};

  auto const modelViewMatrix{
      glm::mat3(m_camera.getViewMatrix() * modelMatrixProt)};
  auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  modelMatrixProt = glm::translate(modelMatrixProt, m_position);
  modelMatrixProt = glm::rotate(modelMatrixProt, m_angle, glm::vec3(0, 1, 0));
  modelMatrixProt = glm::scale(modelMatrixProt, glm::vec3(0.15f));

  // Set uniform variable
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrixProt[0][0]);

  m_protagonist.render();

  // Draw a grid of 2N+1 x 2N+1 tiles on the xz plane, centered around the
  // origin
  auto const N{20};
  for (auto const z : iter::range(-N, N + 1)) {
    for (auto const x : iter::range(-N, N + 1)) {
      // Set model matrix as a translation matrix
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x, 0.0f, z));
      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      m_ground.render();

      if (z == -N || z == N || x == -N || x == N) {
        glm::mat4 model1{1.0f};
        glm::mat4 model2{1.0f};
        model1 = glm::translate(model1, glm::vec3(x, 1.15f, z));
        model2 = glm::translate(model2, glm::vec3(x, 2.30f, z));
        abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model1[0][0]);
        m_sand.render();
        abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model2[0][0]);
        m_sand.render();
      }
    }
  }

  abcg::glUseProgram(0);
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.resizeViewport(size);
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_protagonist.destroy();
  abcg::glDeleteProgram(m_program_obj);
}