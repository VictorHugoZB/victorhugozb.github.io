#include "window.hpp"
#include "abcgOpenGLFunction.hpp"
#include "block.hpp"
#include "core.h"
#include "gamedata.hpp"
#include "imgui.h"
#include <algorithm>

void Window::nextLevel() {
  if (m_gameData.m_level == Level::Level1) {
    m_gameData.m_level = Level::Level2;
  } else if (m_gameData.m_level == Level::Level2) {
    m_gameData.m_level = Level::Level3;
  } else {
    m_gameData.m_level = Level::Level1;
  }
}

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
  m_blocks.create(m_program, m_gameData.m_level);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 3) {
    if (m_gameData.m_state == State::GameOver) {
      m_gameData.m_level = Level::Level1;
    } else {
      this->nextLevel();
    }
    restart();
    return;
  }

  m_bar.update(m_gameData, deltaTime);
  m_border.update();
  m_ball.update(m_bar, m_gameViewport, deltaTime);
  m_blocks.update(m_ball);

  if (m_gameData.m_state == State::Playing) {
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
  m_blocks.paint();
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
      if (m_gameData.m_level == Level::Level1 ||
          m_gameData.m_level == Level::Level2) {
        ImGui::Text("Get ready...");
      } else {
        ImGui::Text("*Congrats!*");
      }
    }

    ImGui::PopFont();
    ImGui::End();
  }

  {
    // Window begin
    auto const size{ImVec2(150, 200)};
    auto const position{ImVec2(m_viewportSize.x - 150, 0)};

    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);

    ImGuiWindowFlags const flags{ImGuiWindowFlags_NoNavFocus |
                                 ImGuiWindowFlags_NoFocusOnAppearing};

    ImGui::Begin("Options", nullptr, flags);
    ImGui::Text("Bar speed");
    ImGui::SliderFloat("  ", &m_bar.m_velocityFactor, 0.2f, 4.00f);
    if (ImGui::Selectable("Level 1", m_gameData.m_level == Level::Level1)) {
      m_gameData.m_level = Level::Level1;
      restart();
    }
    if (ImGui::Selectable("Level 2", m_gameData.m_level == Level::Level2)) {
      m_gameData.m_level = Level::Level2;
      restart();
    }
    if (ImGui::Selectable("Level 3", m_gameData.m_level == Level::Level3)) {
      m_gameData.m_level = Level::Level3;
      restart();
    }

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
  abcg::glDeleteProgram(m_program_ball);

  m_bar.destroy();
  m_border.destroy();
  m_ball.destroy();
  m_blocks.destroy();
}

void Window::checkEndGameConditions() {
  if (m_ball.m_dead) {
    m_ball.m_dead = false;
    m_gameData.m_state = State::GameOver;
    m_restartWaitTimer.restart();
  }

  std::vector<Blocks::Block> m_blocks_destructable;
  std::copy_if(m_blocks.m_blocks.begin(), m_blocks.m_blocks.end(),
               std::back_inserter(m_blocks_destructable),
               [](const Blocks::Block &m) { return m.m_destructable; });

  if (m_blocks_destructable.empty()) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}