project(breakout)
add_executable(${PROJECT_NAME} main.cpp window.cpp bar.cpp border.cpp ball.cpp)
enable_abcg(${PROJECT_NAME})
