#include "headers.h"
#include <bitset>

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
  bool useLocalCamera;

  float updateTime;
  bool running;
  bool showOrbits;
  float time;
  std::bitset<350> pressed;
  void processKey(int key, bool* val);
public:
  GLFWwindow* window;
  std::vector<CelestialBody*> bodies;
  Universe(double g, double timeScale, double updateTime, GLuint width, GLuint height);
  ~Universe();
  void render();
  void update();
  void calculate();
  void processInput();
};
#endif
