#include <iostream>
#include "Program.hpp"

//#include <glm/glm.hpp>
//using namespace glm;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    log(ERROR, "Usage: <config file path>");
    return 1;
  }
  
  try {
    Program p(argv[0], argv[1], 1024, 768, 16);
    p.startMainLoop();
  } catch (std::string error) {
    log(ERROR, error);
    return 1;
  }
}
