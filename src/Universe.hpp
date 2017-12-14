#include "CelestialBody.hpp"

#ifndef UNIVERSE_H
#define UNIVERSE_H

class Universe {
  std::vector<CelestialBody*> bodies;
  glm::mat4 view;
  glm::mat4 projection;
  float updateTime;
  bool running;
  float time;
  bool pressed_space;
public:
  Universe(GLuint width, GLuint height, float updateTime);
  ~Universe();
  void render();
  void update();
  void calculate();
  void processInput(GLFWwindow* window);
};
#endif
