#ifndef SPHERE_H
#define SPHERE_H

#include "gl_headers.hpp"

class Sphere {
  GLuint vertexArray = 0;
  GLuint vertexBuffer = 0;
  GLuint indexBuffer = 0;
  GLuint matrixID = 0;
  glm::mat4 model;
public:
  Sphere(GLuint matrixID);
  ~Sphere();
  void render(glm::mat4 view, glm::mat4 projection);
};
#endif
