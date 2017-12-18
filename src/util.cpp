#include "util.hpp"
#include <iostream>

void vprint(glm::vec3 vector) {
  std::cout << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
}


glm::vec3 midpoint(glm::vec3 a, glm::vec3 b) {
  return (a + b) * 0.5f;
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
