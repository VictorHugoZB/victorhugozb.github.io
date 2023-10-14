#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Down, Up, Fire };
enum class State { Playing, GameOver, Win };
enum class Level { Level1, Level2, Level3 };

struct GameData {
  Level m_level{Level::Level1};
  State m_state{State::Playing};
  std::bitset<5> m_input; // [fire, up, down, left, right]
};

#endif