#include "CelestialBody.hpp"
#include <iostream>

CelestialBody::CelestialBody(MainShader* mainShader, double physicsScale, size_t orbitSize,
			     glm::dvec3 position, glm::dvec3 velocity, double mass,
			     glm::vec3 color, float radius, float rotation, float obliquity,
			     Material material, Texture *texture):
  isStar(false), mainShader(mainShader), physicsScale(physicsScale),  material(material), orbitSize(orbitSize),
  position(position), velocity(velocity), mass(mass), texture(texture) {
  sphere = new Sphere(mainShader, color, radius, rotation, glm::radians(obliquity));
  sphere->update(position * physicsScale, 1.0);
  positions.push_front(position * physicsScale);
  initGL();
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
  positions.push_front(position * physicsScale);
  if (positions.size() > orbitSize) {
    positions.pop_back();
  }
}

void CelestialBody::renderOrbit() {
  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * positions.size(), &positions[0]);
  glm::vec3* map = (glm::vec3*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);  
  for (size_t i = 0; i < positions.size(); i++) {
    map[i] = positions[i];
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glDrawElements(GL_LINE_STRIP, positions.size(),
  		 GL_UNSIGNED_INT, (void*) 0);
}

void CelestialBody::initGL() {
  for (unsigned int i = 0; i <= orbitSize; i++) {
    indices.push_back(i);
  }
  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);
  
  // VBO
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * orbitSize, NULL, GL_STREAM_DRAW);

  // link VBO and attribute information into the VAO
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(VERTEX_ID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // EBO
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

 
