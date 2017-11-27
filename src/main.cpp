#include <iostream>
#include "Program.hpp"

//#include <glm/glm.hpp>
//using namespace glm;

int main(int argc, char* argv[]) {
  try {
    Program p("Planet Sim");
    p.startMainLoop();
  } catch (const char*& error) {
    std::cerr << "[Program][ERROR]: " << error << std::endl;
    return 1;
  }
}
