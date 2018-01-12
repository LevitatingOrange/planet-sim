#include "CelestialBody.hpp"

#ifndef STAR_H
#define STAR_H
class Star: public CelestialBody {
public:
  LightID light_id;
  Light light;
  Star(GLuint program, double physicsScale, glm::dvec3 position, glm::dvec3 velocity,
       double mass, glm::vec3 color,
       float radius, float rotation, float obliquity,
       Material material, Light light, Texture *texture);
  void render(glm::vec3 viewPosition);
  
};
#endif
