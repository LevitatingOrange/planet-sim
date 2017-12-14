#ifndef SPHERE_H
#define SPHERE_H

#include "gl_headers.hpp"
#include "util.hpp"
#include <vector>

// TODO: implement that planets are not perfect spheres

class Sphere {
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
  void spherify();
  void generateVertices();
  void createShape();
public:
  glm::vec3 position;
  float radius;
  float rotation;
  
  Sphere(GLuint matrixID, glm::vec3 position, float radius);
  ~Sphere();
  void render(glm::mat4 view, glm::mat4 projection);
  void update();
};
#endif
