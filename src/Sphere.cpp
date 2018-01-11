#include "Sphere.hpp"
#include <iostream>

void Sphere::initGL() {
  // VAO
  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);
  
  // VBO
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // link VBO and attribute information into the VAO
  glEnableVertexAttribArray(VERTEX_ID);
  glVertexAttribPointer(VERTEX_ID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // EBO
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

Sphere::Sphere(GLuint program, glm::vec3 color, unsigned int detail, float radius, float rotation_angle, float obliquity) :
  model_id(glGetUniformLocation(program, "model")), radius_id(glGetUniformLocation(program, "radius")), color(color), detail(detail),
  rotation(glm::rotate(glm::mat4(1.0f), (float) M_PI/2, glm::vec3(1,0,0))),
  translation(glm::mat4(1.0f)), old_position(glm::vec3(0,0,0)), radius(radius), rotation_angle(rotation_angle), obliquity(obliquity) {
  rotation = glm::rotate(rotation, obliquity, glm::vec3(0,0,1));
  initGL();
}

Sphere::~Sphere() {
  if (vertexArray != 0) {
    glDeleteVertexArrays(1, &vertexArray);
  }
  if (vertexBuffer != 0) {
    glDeleteBuffers(1, &vertexBuffer);
  }
}

void Sphere::render() {
  glm::mat4 model = translation * rotation;
  glUniformMatrix4fv(model_id, 1, GL_FALSE, &model[0][0]);
  glUniform1f(radius_id, radius);
  
  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glPatchParameteri(GL_PATCH_VERTICES, 3);
  glDrawElements(GL_PATCHES, sizeof(indices)/(sizeof(int)), GL_UNSIGNED_INT, (void*)0);
}

void Sphere::update(glm::vec3 position, double scale) {
  translation = glm::translate(translation, old_position - position);
  rotation = glm::rotate(rotation, rotation_angle * (float) scale, glm::vec3(0,0,1));
  old_position = position;
}

