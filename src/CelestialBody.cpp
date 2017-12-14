#include "CelestialBody.hpp"

CelestialBody::CelestialBody(glm::vec3 position, glm::vec3 velocity, float mass,
			     unsigned int detail, float radius, float rotation, float obliquity):
  position(position), velocity(velocity), mass(mass) {
  sphere = new Sphere(detail, radius, rotation, glm::radians(obliquity));
  sphere->update(position);
}

CelestialBody::~CelestialBody() {
  delete sphere;
}
void CelestialBody::render(glm::mat4 view, glm::mat4 projection) {
  sphere->render(view, projection);
}
void CelestialBody::update() {
  sphere->update(position);
}
 
