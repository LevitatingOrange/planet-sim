#include <iostream>
#include "Program.hpp"

//#include <glm/glm.hpp>
//using namespace glm;

int main(int argc, char* argv[]) {
  try {
    Program p("Planet Sim", "./shaders/vertex.glsl", "./shaders/fragment.glsl", 1024, 768, 16);
    p.startMainLoop();
  } catch (std::string error) {
    std::cerr << "[Program][ERROR]: " << error << std::endl;
    return 1;
  }
}
