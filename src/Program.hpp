#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>

class Program {
  GLFWwindow* window;
public:
  Program(std::string name);
  void startMainLoop();
  ~Program();
};
#endif
