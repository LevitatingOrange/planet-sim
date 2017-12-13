#ifndef SPHERE_H
#define SPHERE_H

#include "gl_headers.hpp"
#include "util.hpp"
#include <vector>

class Sphere {
  float radius;
  unsigned int detail;
  GLuint vertexArray = 0;
  GLuint vertexBuffer = 0;
  std::vector<glm::vec3> vectors;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  GLuint indexBuffer = 0;
  GLuint matrixID = 0;
  glm::mat4 model;
  void initGL();
  void generateMesh();
  void subdivide(glm::vec3 a, glm::vec3 b, glm::vec3 c, int depth);
  void reduce();
  void generateVertices();
public:
  Sphere(GLuint matrixID);
  ~Sphere();
  void render(glm::mat4 view, glm::mat4 projection, float angleX);
};
#endif
