#include <iostream>
#include "Program.hpp"
#include "util.hpp"

//#include <glm/glm.hpp>
//using namespace glm;

int main(int argc, char* argv[]) {
  if (argc != 6) {
    log(ERROR, "Usage: <vertex shader path> <tessellation control shader path> <tessellation eval shader path> <fragment shader path> <config file path>");
    return 1;
  }
  
  try {
    Program p("Planet Sim", argv[1], argv[2], argv[3], argv[4], argv[5], 1024, 768, 16);
    p.startMainLoop();
  } catch (std::string error) {
    log(ERROR, error);
    return 1;
  }
}
