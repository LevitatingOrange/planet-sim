#include "Star.hpp"

Star::Star(GLuint program, glm::vec3 position, glm::vec3 velocity,
	   float mass, glm::vec3 color, unsigned int detail,
	   float radius, float rotation, float obliquity,
	   Material material, Light light, Texture *texture):
  CelestialBody(program, position, velocity, mass, color, detail, radius, rotation, obliquity, material, texture),
  light_id(LightID {
      glGetUniformLocation(program, "light.ambient"),
	glGetUniformLocation(program, "light.diffuse"),
	glGetUniformLocation(program, "light.specular")}), light(light) {
}

void Star::render() {
  glUniform3fv(light_id.ambient, 1, (GLfloat*) &light.ambient);
  glUniform3fv(light_id.diffuse, 1, (GLfloat*) &light.diffuse);
  glUniform3fv(light_id.specular, 1, (GLfloat*) &light.specular);
  //glUniform3fv(, 1, (GLfloat*) &eye);
  CelestialBody::render();
}
