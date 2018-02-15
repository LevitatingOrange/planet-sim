#include "headers.h"

#include <iostream>
#include <exception>
#include <vector>

#ifndef PROGRAM_H
#define PROGRAM_H

#define FPS_DISPLAY_SMOOTHING 0.9

class Program {
  GLFWwindow* window;
  float updateTime;
public:
  Universe* universe;
  Text* fpsDisplay;
  Program(std::string name, std::string config_path, GLuint width, GLuint height, float updateTime);
  void startMainLoop();
  ~Program();
};
#endif
