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
float angle_clip(float angle, float increase);
std::string readFile(const char* filePath);
GLuint createProgram(const char *vertex_path, const char* tess_control_path, const char* tess_eval_path, const char* fragment_path);
#endif

