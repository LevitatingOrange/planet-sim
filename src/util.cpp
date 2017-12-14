#include "util.hpp"


const float epsilon = 0.00001;


bool vequal(glm::vec3 a, glm::vec3 b) {
  return glm::all(glm::epsilonEqual(a, b, epsilon));
}

glm::vec3 midpoint(glm::vec3 a, glm::vec3 b) {
  return (a + b) * 0.5f;
}

glm::vec3 normalize(glm::vec3 a, glm::vec3 b, float length) {
  return a + ((b - a) * length / glm::distance(a, b));
}

