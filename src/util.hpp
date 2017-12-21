#include <vector>
#include <string>
#include <tuple>
#include <glm/gtc/epsilon.hpp>
#include <glm/glm.hpp>

#ifndef UTIL_H
#define UTIL_H

typedef enum _ErrorLevel {
  INFO,
  WARNING,
  ERROR,
} ErrorLevel;

typedef struct _Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;
} Vertex;

void log(ErrorLevel lvl, std::string msg);
void vprint(glm::vec3 vector);
glm::vec3 midpoint(glm::vec3 a, glm::vec3 b);
glm::vec3 normalize(glm::vec3 a, glm::vec3 b, float length);
float angle_clip(float angle, float increase);
#endif

