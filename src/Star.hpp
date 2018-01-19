#include "headers.h"

#ifndef STAR_H
#define STAR_H
class Star: public CelestialBody {
public:
  Light light;
  Star(MainShader* mainShader, double physicsScale, glm::dvec3 position, glm::dvec3 velocity,
       double mass, glm::vec3 color,
       float radius, float rotation, float obliquity,
       Material material, Light light, Texture *texture);
  void render(glm::vec3 viewPosition);
};
#endif
