#include "Sphere.hpp"

#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

class CelestialBody {
public:
  glm::vec3 position;
  glm::vec3 velocity;
  double mass;
  Sphere *sphere;

  CelestialBody(glm::vec3 position, glm::vec3 velocity, double mass, unsigned int detail, double radius, double rotation, double obliquity);
  ~CelestialBody();
  void render(glm::mat4 view, glm::mat4 projection);
  void update();
};

#endif
