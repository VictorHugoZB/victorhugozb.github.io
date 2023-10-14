#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left };
enum class State { Playing, GameOver, Win };

struct GameData {
  State m_state{State::Playing};
  std::bitset<5> m_input;
};

struct GameViewport {
  double x;
  double y;
  double width;
  double height;
};

#endif