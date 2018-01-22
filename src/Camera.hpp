#include "headers.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
protected:
  MainShader* mainShader;
  std::vector<CelestialBody*>* bodies;
public:
  glm::mat4 view;
  glm::vec3 eye;
public:
  Camera(MainShader* mainShader, std::vector<CelestialBody*>* bodies);
  virtual void render() = 0;
  virtual void processInput(GLFWwindow* window) = 0;
};

class GlobalCamera: public Camera {
  // mouse
  float sensitivity;
  float lastX;
  float lastY;
private:
  glm::vec3 direction;
  glm::vec3 camera_up;
  float yaw;
  float pitch;
  float speed_modifier;
  float speed;
  glm::vec3 up;

  bool pressed_r;
public:
  GlobalCamera(MainShader* mainShader, std::vector<CelestialBody*>* bodies);
  ~GlobalCamera();
  virtual void render();
  virtual void processInput(GLFWwindow *window);
};

// class LocalCamera: Camera {
// public:
//   LocalCamera();
//   ~LocalCamera();
// };

#endif
