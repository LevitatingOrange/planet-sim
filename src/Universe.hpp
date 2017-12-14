#include "Sphere.hpp"

#ifndef UNIVERSE_H
#define UNIVERSE_H

class Universe {
  std::vector<Sphere*> spheres;
  glm::mat4 view;
  glm::mat4 projection;
  double updateTime;
  bool running;
  double time;
  bool pressed_space;
public:
  Universe(GLuint width, GLuint height, double updateTime);
  ~Universe();
  void render();
  void update();
  void processInput(GLFWwindow* window);
};
#endif
