#include "Sphere.hpp"

#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#define G 6.674e-11

class CelestialBody {
public:
  glm::vec3 position;
  glm::vec3 velocity;
  float mass;
  Sphere *sphere;

  CelestialBody(glm::vec3 position, glm::vec3 velocity, float mass, unsigned int detail, float radius, float rotation, float obliquity);
  ~CelestialBody();
  void render(glm::mat4 view, glm::mat4 projection);
  void update();
};

#endif
