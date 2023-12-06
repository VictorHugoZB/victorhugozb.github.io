project(idk)
add_executable(${PROJECT_NAME} camera.cpp ground.cpp main.cpp window.cpp protagonist.cpp trackball.cpp)
enable_abcg(${PROJECT_NAME})
