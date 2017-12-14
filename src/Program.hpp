#include "gl_headers.hpp"

#include <iostream>
#include <exception>
#include <vector>
#include "Universe.hpp"

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
  GLFWwindow* window;
  Universe* universe;
public:
  Program(std::string name, std::string vertex_shader, std::string fragment_shader, GLuint width, GLuint height);
  void startMainLoop();
  ~Program();
};
#endif
