#include "Star.hpp"

Star::Star(GLuint program, double physicsScale, glm::dvec3 position, glm::dvec3 velocity,
	   double mass, glm::vec3 color,
	   float radius, float rotation, float obliquity,
	   Material material, Light light, Texture *texture):
  CelestialBody(program, physicsScale, position, velocity, mass, color,
		radius, rotation, obliquity, material, texture),
  light_id(LightID {
      glGetUniformLocation(program, "light.ambient"),
	glGetUniformLocation(program, "light.diffuse"),
	glGetUniformLocation(program, "light.specular")}), light(light) {
}

void Star::render(glm::vec3 viewPosition) {
  glUniform3fv(light_id.ambient, 1, (GLfloat*) &light.ambient);
  glUniform3fv(light_id.diffuse, 1, (GLfloat*) &light.diffuse);
  glUniform3fv(light_id.specular, 1, (GLfloat*) &light.specular);
  //glUniform3fv(, 1, (GLfloat*) &eye);
  CelestialBody::render(viewPosition);
}
