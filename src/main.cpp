#include <iostream>
#include "Program.hpp"
#include "util.hpp"

//#include <glm/glm.hpp>
//using namespace glm;

int main(int argc, char* argv[]) {
  if (argc != 4) {
    log(ERROR, "Usage: <vertex shader path> <fragment shader path> <config file path>");
    return 1;
  }
  
  try {
    Program p("Planet Sim", argv[1], argv[2], argv[3], 1024, 768, 16);
    p.startMainLoop();
  } catch (std::string error) {
    log(ERROR, error);
    return 1;
  }
}
