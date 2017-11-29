#ifndef PROGRAM_H
#define PROGRAM_H

#include "gl_headers.hpp"

#include <iostream>
#include <exception>
#include <vector>
#include "Sphere.hpp"

class Program {
  GLFWwindow* window;
  std::vector<Sphere*> spheres;
  glm::mat4 view;
  glm::mat4 projection;
public:
  Program(std::string name, std::string vertex_shader, std::string fragment_shader, GLuint width, GLuint height);
  void startMainLoop();
  ~Program();
};
#endif
