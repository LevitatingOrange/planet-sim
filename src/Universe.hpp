#include "CelestialBody.hpp"

#ifndef UNIVERSE_H
#define UNIVERSE_H

class Universe {
  GLuint projection_id;
  GLuint view_id;
  GLuint view_position_id;
  
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
  bool pressed_escape;
  bool pressed_r;

public:
  std::vector<CelestialBody*> bodies;
  Universe(float g, GLuint program, float updateTime, GLuint width, GLuint height);
  ~Universe();
  void render();
  void update();
  void calculate();
  void processInput(GLFWwindow* window);
};
#endif
