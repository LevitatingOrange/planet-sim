#include "Sphere.hpp"

#ifndef UNIVERSE_H
#define UNIVERSE_H

class Universe {
  std::vector<Sphere*> spheres;
  glm::mat4 view;
  glm::mat4 projection;
public:
  Universe(GLuint matrixID, GLuint width, GLuint height);
  ~Universe();
  void render();
  void update();
  void handleKey(int key);
};
#endif
