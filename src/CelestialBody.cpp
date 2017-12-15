#include "CelestialBody.hpp"

CelestialBody::CelestialBody(glm::vec3 position, glm::vec3 velocity, double mass,
			     unsigned int detail, double radius, double rotation, double obliquity):
  position(position), velocity(velocity), mass(mass) {
  sphere = new Sphere(detail, position, radius, rotation, glm::radians(obliquity));
}

CelestialBody::~CelestialBody() {
  delete sphere;
}
void CelestialBody::render(glm::mat4 view, glm::mat4 projection) {
  sphere->render(view, projection);
}
void CelestialBody::update() {
  sphere->update();
}
 
