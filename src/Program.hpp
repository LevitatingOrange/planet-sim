#include "headers.h"

#include <iostream>
#include <exception>
#include <vector>

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
GLFWwindow* window;
Universe* universe;
float updateTime;
public:
  Program(std::string name, std::string config_path, GLuint width, GLuint height, float updateTime);
  void startMainLoop();
  ~Program();
};
#endif
