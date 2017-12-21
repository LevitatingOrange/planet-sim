#include "CelestialBody.hpp"

CelestialBody::CelestialBody(GLuint program, glm::vec3 position, glm::vec3 velocity, float mass,
			     glm::vec3 color, unsigned int detail, float radius, float rotation, float obliquity,
			     Parameter material, float shininess):
  material_id(ParameterID {
      glGetUniformLocation(program, "materialParameters.ambient"),
	glGetUniformLocation(program, "materialParameters.diffuse"),
	glGetUniformLocation(program, "materialParameters.specular"),
	}),  material(material),  shininess_id(glGetUniformLocation(program, "shininess")), shininess(shininess), 
  position(position), velocity(velocity), mass(mass) {
  sphere = new Sphere(program, color, detail, radius, rotation, glm::radians(obliquity));
  sphere->update(position);
}

CelestialBody::~CelestialBody() {
  delete sphere;
}
void CelestialBody::render() {
  glUniform3fv(material_id.ambient, 1, (GLfloat*) &material.ambient);
  glUniform3fv(material_id.diffuse, 1, (GLfloat*) &material.diffuse);
  glUniform3fv(material_id.specular, 1, (GLfloat*) &material.specular);
  glUniform1fv(shininess_id, 1, (GLfloat*) &shininess);
  sphere->render();
}
void CelestialBody::update() {
  sphere->update(position);
}
 
