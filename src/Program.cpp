#include "Program.hpp"

Program::Program(std::string name) {
  if(!glfwInit()) {
    throw "Failed to initialize GLFW";
  }
  
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( 1024, 768, name.c_str(), NULL, NULL);
  if (window == NULL) {
    glfwTerminate();
    throw "Failed to open GLFW window";
  }
  
  glfwMakeContextCurrent(window); // Initialize GLEW
  glewExperimental=true; // Needed in core profile
  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    throw "Failed to initialize GLEW";
  }
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void Program::startMainLoop() {
  do {
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

Program::~Program() {
  std::cout << "Program terminated" << std::endl;
}
