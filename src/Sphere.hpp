#include "gl_headers.hpp"
#include "util.hpp"
#include <vector>

#ifndef SPHERE_H
#define SPHERE_H

#define VERTEX_ID 0
#define NORMAL_ID 1
#define COLOR_ID 2
#define TEXTURE_ID 3

typedef struct _Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;
  glm::vec2 texture;
} Vertex;
// TODO: implement ellipsoids

class Sphere {
  GLuint model_id;
  
  GLuint vertexArray = 0;
  GLuint vertexBuffer = 0;
  std::vector<Vertex> vertices;
  std::vector<glm::vec3> vectors;
  std::vector<GLuint> indices;
  GLuint indexBuffer = 0;
  glm::vec3 color;
  unsigned int detail;
  glm::mat4 rotation;
  glm::mat4 translation;
  glm::vec3 old_position;
  void initGL();
  void generateMesh();
  void subdivide(glm::vec3 a, glm::vec3 b, glm::vec3 c, int depth);
  void reduce();
  void spherify();
  void generateVertices();
  void createShape();
public:
  // physical traits
  float radius;
  float rotation_angle;
  float obliquity;
  
  Sphere(GLuint program, glm::vec3 color, unsigned int detail, float radius, float rotation_angle, float obliquity);
  ~Sphere();
  void render();
  void update(glm::vec3 position, double scale);
};
#endif
