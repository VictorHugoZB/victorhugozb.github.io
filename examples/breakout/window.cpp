#include "window.hpp"
#include "abcgOpenGLFunction.hpp"
#include "core.h"

void Window::onEvent(SDL_Event const &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Right));
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Load a new font
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  // Create program to render the other objects
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "objects.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "objects.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_program_ball = abcg::createOpenGLProgram(
      {{.source = assetsPath + "ball.vert", .stage = abcg::ShaderStage::Vertex},
       {.source = assetsPath + "ball.frag",
        .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void Window::restart() {
  m_gameData.m_state = State::Playing;

  m_bar.create(m_program);
  m_border.create(m_program);
  m_ball.create(m_program_ball);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 3) {
    restart();
    return;
  }

  m_bar.update(m_gameData, deltaTime);
  m_border.update();
  m_ball.update(m_bar, m_gameViewport, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkEndGameConditions();
  }
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(m_gameViewport.x, m_gameViewport.y, m_gameViewport.width,
                   m_gameViewport.height);

  m_bar.paint(m_gameData);
  m_border.paint(m_gameData);
  m_ball.paint();
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const size{ImVec2(300, 85)};
    auto const position{ImVec2((m_viewportSize.x - size.x) / 2.0f,
                               (m_viewportSize.y - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_gameViewport.x = (size.x - size.y * 0.67) / 2;
  m_gameViewport.y = 5;
  m_gameViewport.width = size.y * 0.67;
  m_gameViewport.height = size.y - 10.00;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteProgram(m_program);

  m_bar.destroy();
  m_border.destroy();
  m_ball.destroy();
}

void Window::checkCollisions() {
  // Check collision between ship and asteroids
  //   for (auto const &asteroid : m_asteroids.m_asteroids) {
  //     auto const asteroidTranslation{asteroid.m_translation};
  //     auto const distance{
  //         glm::distance(m_ship.m_translation, asteroidTranslation)};

  //     if (distance < m_ship.m_scale * 0.9f + asteroid.m_scale * 0.85f) {
  //       m_gameData.m_state = State::GameOver;
  //       m_restartWaitTimer.restart();
  //     }
  //   }

  //   // Check collision between bullets and asteroids
  //   for (auto &bullet : m_bullets.m_bullets) {
  //     if (bullet.m_dead)
  //       continue;

  //     for (auto &asteroid : m_asteroids.m_asteroids) {
  //       for (auto const i : {-2, 0, 2}) {
  //         for (auto const j : {-2, 0, 2}) {
  //           auto const asteroidTranslation{asteroid.m_translation +
  //                                          glm::vec2(i, j)};
  //           auto const distance{
  //               glm::distance(bullet.m_translation, asteroidTranslation)};

  //           if (distance < m_bullets.m_scale + asteroid.m_scale * 0.85f) {
  //             asteroid.m_hit = true;
  //             bullet.m_dead = true;
  //           }
  //         }
  //       }
  //     }

  //     // Break asteroids marked as hit
  //     for (auto const &asteroid : m_asteroids.m_asteroids) {
  //       if (asteroid.m_hit && asteroid.m_scale > 0.10f) {
  //         std::uniform_real_distribution randomDist{-1.0f, 1.0f};
  //         std::generate_n(std::back_inserter(m_asteroids.m_asteroids), 3,
  //         [&]() {
  //           glm::vec2 const offset{randomDist(m_randomEngine),
  //                                  randomDist(m_randomEngine)};
  //           auto const newScale{asteroid.m_scale * 0.5f};
  //           return m_asteroids.makeAsteroid(
  //               asteroid.m_translation + offset * newScale, newScale);
  //         });
  //       }
  //     }

  //     m_asteroids.m_asteroids.remove_if([](auto const &a) { return a.m_hit;
  //     });
  //   }
}

void Window::checkEndGameConditions() {
  if (m_ball.m_dead) {
    m_ball.m_dead = false;
    m_gameData.m_state = State::GameOver;
    m_restartWaitTimer.restart();
  }
}