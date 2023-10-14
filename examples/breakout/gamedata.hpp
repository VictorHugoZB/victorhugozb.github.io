#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left };
enum class State { Playing, GameOver, Win };
enum class Level { Level1, Level2, Level3 };

struct GameData {
  Level m_level{Level::Level1};
  State m_state{State::Playing};
  std::bitset<2> m_input;
};

struct GameViewport {
  double x;
  double y;
  double width;
  double height;
};

#endif