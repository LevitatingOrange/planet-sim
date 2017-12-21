#include "CelestialBody.hpp"

#ifndef STAR_H
#define STAR_H
class Star: public CelestialBody {
public:
  LightID light_id;
  Light light;
  Star(GLuint program, glm::vec3 position, glm::vec3 velocity,
       float mass, glm::vec3 color, unsigned int detail,
       float radius, float rotation, float obliquity,
       Material material, Light light, Texture *texture);
  void render();
  
};
#endif
