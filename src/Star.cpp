#include "Star.hpp"

Star::Star(MainShader* mainShader, double physicsScale, size_t orbitSize, glm::dvec3 position, glm::dvec3 velocity,
	   double mass, glm::vec3 color,
	   float radius, float rotation, float obliquity,
	   Material material, Light light, Texture *texture):
  CelestialBody(mainShader, physicsScale, orbitSize, position, velocity, mass, color,
		radius, rotation, obliquity, material, texture), light(light) {
  isStar = true;
}

void Star::setLights(glm::vec3 viewPosition, size_t i) {
  light.position = position * physicsScale;
  mainShader->setLight(light, i);
}
