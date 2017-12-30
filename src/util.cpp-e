#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

void log(ErrorLevel lvl, std::string msg) {
  switch (lvl) {
  case INFO:
    std::cout << "[Program][INFO]: ";
      break;
  case WARNING:
    std::cout << "[Program][WARNING]: ";
    break;
  case ERROR:
    std::cout << "[Program][ERROR]: ";
    break;
  }
  std::cout << msg << std::endl;
}

void vprint(glm::vec3 vector) {
  std::cout << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
}

glm::vec3 normalize(glm::vec3 a, glm::vec3 b, float length) {
  return a + ((b - a) * length / glm::distance(a, b));
}

float angle_clip(float angle, float increase) {
  if (increase > 0) {
    if (angle >= 2 * M_PI) {
      return increase;
    } else {
      return angle + increase;
    }
  }
  else {
    if (angle <= 0) {
      return 2 * M_PI + increase;
    } else {
      return angle + increase;
    }
  }
}

std::string readFile(const char* filePath) {
  std::ifstream fileStream(filePath, std::ios::in);

  if (!fileStream.is_open()) {
    throw std::string("Could not read file");
  }

  std::string content( (std::istreambuf_iterator<char>(fileStream) ),
                       (std::istreambuf_iterator<char>()    ) );
  
  return content;
}
