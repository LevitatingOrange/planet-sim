#include "Star.hpp"

Star::Star(MainShader* mainShader, double physicsScale, glm::dvec3 position, glm::dvec3 velocity,
	   double mass, glm::vec3 color,
	   float radius, float rotation, float obliquity,
	   Material material, Light light, Texture *texture):
  CelestialBody(mainShader, physicsScale, position, velocity, mass, color,
		radius, rotation, obliquity, material, texture), light(light) {
}

void Star::render(glm::vec3 viewPosition) {
  mainShader->setLight(light);
  CelestialBody::render(viewPosition);
}
