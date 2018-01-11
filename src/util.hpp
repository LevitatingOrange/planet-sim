#include <vector>
#include <string>
#include <tuple>
#include "gl_headers.hpp"
#include <glm/gtc/epsilon.hpp>
#include <glm/glm.hpp>

#ifndef UTIL_H
#define UTIL_H

typedef enum _ErrorLevel {
  INFO,
  WARNING,
  ERROR,
} ErrorLevel;

void log(ErrorLevel lvl, std::string msg);
void vprint(glm::vec3 vector);
void vdprint(glm::dvec3 vector);
template <typename T>
inline T midpoint(T a, T b) {
  return (a + b) * 0.5f;
}
glm::vec3 normalize(glm::vec3 a, glm::vec3 b, float length);
float angle_clip(float angle, float increase);
std::string readFile(const char* filePath);
GLuint loadShader(const char *vertex_path, const char* fragment_path);
#endif

