#include "CelestialBody.hpp"

CelestialBody::CelestialBody(GLuint program, glm::vec3 position, glm::vec3 velocity, float mass,
			     glm::vec3 color, unsigned int detail, float radius, float rotation, float obliquity,
			     Material material, Texture *texture):
  material_id(MaterialID {
      glGetUniformLocation(program, "material.ambient"),
	glGetUniformLocation(program, "material.diffuse"),
	glGetUniformLocation(program, "material.specular"),
	glGetUniformLocation(program, "material.shininess"),
	glGetUniformLocation(program, "material.nonLambertian"),
	}),  material(material), use_texture_id(glGetUniformLocation(program, "useTexture")),
  position(position), velocity(velocity), mass(mass), texture(texture) {
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
  glUniform1f(material_id.shininess, (GLfloat) material.shininess);
  glUniform1i(material_id.nonLambertian, (GLint) material.nonLambertian);

  if (texture != nullptr) {
    glUniform1i(use_texture_id, 1);
    texture->render();
  } else {
    glUniform1i(use_texture_id, 0);
  }
  sphere->render();
}
void CelestialBody::update() {
  sphere->update(position);
}
 
