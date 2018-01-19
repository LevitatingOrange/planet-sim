#include "CelestialBody.hpp"

CelestialBody::CelestialBody(MainShader* mainShader, double physicsScale, glm::dvec3 position, glm::dvec3 velocity, double mass,
			     glm::vec3 color, float radius, float rotation, float obliquity,
			     Material material, Texture *texture):
  mainShader(mainShader), physicsScale(physicsScale),  material(material),
  position(position), velocity(velocity), mass(mass), texture(texture) {
  sphere = new Sphere(mainShader, color, radius, rotation, glm::radians(obliquity));
  sphere->update(position * physicsScale, 1.0);
}

CelestialBody::~CelestialBody() {
  delete sphere;
}
void CelestialBody::render(glm::vec3 viewPosition) {
  mainShader->setMaterial(material);
  mainShader->setUseTexture(texture != nullptr);
  if (texture != nullptr) {
    texture->render();
  }
  sphere->render(viewPosition);
}
void CelestialBody::update(double timeScale) {
  sphere->update(position * physicsScale, timeScale);
}
 
