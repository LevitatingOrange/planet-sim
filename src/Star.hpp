#include "headers.h"

#ifndef STAR_H
#define STAR_H
class Star: public CelestialBody {
public:
  Light light;
  Star(std::string name, MainShader* mainShader, double physicsScale, size_t orbitSize, glm::dvec3 position, glm::dvec3 velocity,
       double mass, glm::vec3 color,
       float radius, float rotation, float obliquity,
       Material material, Light light, Texture *texture);
  void setLights(glm::vec3 viewPosition, size_t i);
};
#endif
