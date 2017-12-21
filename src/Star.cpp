#include "Star.hpp"

Star::Star(GLuint program, glm::vec3 position, glm::vec3 velocity,
	   float mass, glm::vec3 color, unsigned int detail,
	   float radius, float rotation, float obliquity,
	   Parameter material, float shininess, Parameter light):
  CelestialBody(program, position, velocity, mass, color, detail, radius, rotation, obliquity, material, shininess),
  light_id(ParameterID {
      glGetUniformLocation(program, "lightParameters.ambient"),
	glGetUniformLocation(program, "lightParameters.diffuse"),
	glGetUniformLocation(program, "lightParameters.specular")}), light(light) {
}

void Star::render() {
  glUniform3fv(light_id.ambient, 1, (GLfloat*) &light.ambient);
  glUniform3fv(light_id.diffuse, 1, (GLfloat*) &light.diffuse);
  glUniform3fv(light_id.specular, 1, (GLfloat*) &light.specular);
  //glUniform3fv(, 1, (GLfloat*) &eye);
  CelestialBody::render();
}
