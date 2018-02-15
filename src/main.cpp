#include <iostream>
#include "Program.hpp"

//#include <glm/glm.hpp>
//using namespace glm;

int main(int argc, char* argv[]) {
  if (argc != 4) {
    log(ERROR, "Usage: <config file path> <width> <height>");
    return 1;
  }
  
  try {
    Program p(argv[0], argv[1], atoi(argv[2]), atoi(argv[3]), 1);
    p.startMainLoop();
  } catch (std::string error) {
    log(ERROR, error);
    return 1;
  }
}
