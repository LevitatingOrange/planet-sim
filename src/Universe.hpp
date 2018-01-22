#include "headers.h"

#ifndef UNIVERSE_H
#define UNIVERSE_H

typedef enum _CameraMode {
  GLOBAL,
  LOCAL
} CameraMode;

class Universe {
public:
  MainShader* mainShader;
  OrbitShader* orbitShader;
private:
  glm::mat4 projection;

  // physics
  double g;
  double timeScale;

  // camera
  Camera* camera;

  float updateTime;
  bool running;
  float time;
  bool pressed_space;
  bool pressed_escape;
  
public:
  std::vector<CelestialBody*> bodies;
  Universe(double g, double timeScale, double updateTime, GLuint width, GLuint height);
  ~Universe();
  void render();
  void update();
  void calculate();
  void processInput(GLFWwindow* window);
};
#endif
