#include "gl_headers.hpp"
#include "util.hpp"
#include <vector>

#ifndef SPHERE_H
#define SPHERE_H

#define VERTEX_ID 0
#define NORMAL_ID 1
#define COLOR_ID 2
#define TEXTURE_ID 3

// coordinates taken from http://prideout.net/blog/?p=48
const int indices[] = {
  2, 1, 0,
  3, 2, 0,
  4, 3, 0,
  5, 4, 0,
  1, 5, 0,
  11, 6,  7,
  11, 7,  8,
  11, 8,  9,
  11, 9,  10,
  11, 10, 6,
  1, 2, 6,
  2, 3, 7,
  3, 4, 8,
  4, 5, 9,
  5, 1, 10,
  2,  7, 6,
  3,  8, 7,
  4,  9, 8,
  5, 10, 9,
  1, 6, 10 };

const float vertices[] = {
  0.000f,  0.000f,  1.000f,
  0.894f,  0.000f,  0.447f,
  0.276f,  0.851f,  0.447f,
  -0.724f,  0.526f,  0.447f,
  -0.724f, -0.526f,  0.447f,
  0.276f, -0.851f,  0.447f,
  0.724f,  0.526f, -0.447f,
  -0.276f,  0.851f, -0.447f,
  -0.894f,  0.000f, -0.447f,
  -0.276f, -0.851f, -0.447f,
  0.724f, -0.526f, -0.447f,
  0.000f,  0.000f, -1.000f
};

// TODO: implement ellipsoids

class Sphere {
  GLuint model_id;
  GLuint radius_id;
  
  GLuint vertexArray = 0;
  GLuint vertexBuffer = 0;
  GLuint indexBuffer = 0;
  glm::vec3 color;
  unsigned int detail;
  glm::mat4 rotation;
  glm::mat4 translation;
  glm::vec3 old_position;
  void initGL();
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
