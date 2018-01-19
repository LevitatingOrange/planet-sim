#include "headers.h"

#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

class CelestialBody {
public:
  MainShader* mainShader;
  
  double physicsScale;
  
  Material material;
  
  glm::dvec3 position;
  glm::dvec3 velocity;
  double mass;
  Sphere *sphere;

  Texture *texture;

  CelestialBody(MainShader* mainShader, double physicsScale, glm::dvec3 position, glm::dvec3 velocity,
		double mass, glm::vec3 color,
		float radius, float rotation, float obliquity,
		Material material, Texture *texture);
  ~CelestialBody();
  virtual void render(glm::vec3 viewPosition);
  virtual void update(double timeScale);
};

#endif
