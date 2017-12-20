#include "CelestialBody.hpp"

#ifndef UNIVERSE_H
#define UNIVERSE_H

class Universe {
  std::vector<CelestialBody*> bodies;
  glm::mat4 view;
  glm::mat4 projection;

  // physics
  float g;

  // mouse
  float sensitivity;
  float lastX;
  float lastY;

  // player
  glm::vec3 eye;
  glm::vec3 direction;
  glm::vec3 camera_up;
  float yaw;
  float pitch;
  float speed_modifier;
  float speed;

  // world
  glm::vec3 up;
  
  float updateTime;
  bool running;
  float time;
  bool pressed_space;
  bool pressed_r;

  // window
  GLuint width;
  GLuint height;
public:
  Universe(GLuint width, GLuint height, float updateTime);
  ~Universe();
  void render();
  void update();
  void calculate();
  void processInput(GLFWwindow* window);
};
#endif
