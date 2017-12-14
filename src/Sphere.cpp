#include "Sphere.hpp"
#include <iostream>

void Sphere::subdivide(glm::vec3 a, glm::vec3 b, glm::vec3 c, int depth) {
  if (depth == 0) {
    vectors.push_back(a);
    vectors.push_back(b);
    vectors.push_back(c);
    indices.push_back(vectors.size() - 3);
    indices.push_back(vectors.size() - 2);
    indices.push_back(vectors.size() - 1);
  } else {
    glm::vec3 m1 = midpoint(a, b);
    glm::vec3 m2 = midpoint(a, c);
    glm::vec3 m3 = midpoint(b, c);
    
    subdivide(a, m1, m2, depth-1);
    subdivide(m1, b, m3, depth-1);
    subdivide(m2, m3, c, depth-1);
    subdivide(m1, m2, m3, depth-1);
  }
}


void Sphere::reduce() {
  std::vector<glm::vec3> new_vectors;
  
  for (size_t i = 0; i < vectors.size(); i++) {
    if (indices[i] == i) {
      glm::vec3 curr = vectors[i];
      new_vectors.push_back(curr);
    
      for (size_t j = 0; j < vectors.size(); j++) {
	glm::vec3 dup = vectors[j];
	if (glm::distance(curr, dup) < 0.001) {
	  indices[j] = new_vectors.size() - 1;
	}
      }
    }
  }
  vectors = new_vectors;
}

void Sphere::generateMesh() {
  subdivide(glm::vec3(-radius, 0, -radius), glm::vec3(0, radius, 0), glm::vec3(radius, 0, -radius), detail);
  subdivide(glm::vec3(radius, 0, -radius), glm::vec3(0, radius, 0), glm::vec3(radius, 0, radius), detail);
  subdivide(glm::vec3(-radius, 0, -radius), glm::vec3(0, radius, 0), glm::vec3(-radius, 0, radius), detail);
  subdivide(glm::vec3(-radius, 0, radius), glm::vec3(0, radius, 0), glm::vec3(radius, 0, radius), detail);
  subdivide(glm::vec3(-radius, 0, -radius), glm::vec3(0, -radius, 0), glm::vec3(radius, 0, -radius), detail);
  subdivide(glm::vec3(radius, 0, -radius), glm::vec3(0, -radius, 0), glm::vec3(radius, 0, radius), detail);
  subdivide(glm::vec3(-radius, 0, -radius), glm::vec3(0, -radius, 0), glm::vec3(-radius, 0, radius), detail);
  subdivide(glm::vec3(-radius, 0, radius), glm::vec3(0, -radius, 0), glm::vec3(radius, 0, radius), detail);
}

void Sphere::spherify() {
  for (size_t i = 0; i < vectors.size(); i++) {
    vectors[i] = normalize(glm::vec3(0,0,0), vectors[i], radius);
  }
}

void Sphere::generateVertices() {
  for (size_t i = 0; i < vectors.size(); i++) {
    vertices.push_back(Vertex {vectors[i], glm::vec3(1.0, 1.0, 1.0)});
  }
}

void Sphere::createShape() {
  vectors.clear();
  vertices.clear();
  indices.clear();
  generateMesh();
  reduce();
  spherify();
  generateVertices();
}

void Sphere::initGL() {
  // VAO
  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);
  
  // VBO
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  // link VBO and attribute information into the VAO
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));

  // EBO
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
  
#ifdef DEBUG
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
  model = glm::mat4(1.0f);
}


Sphere::Sphere(GLuint matrixID) : matrixID(matrixID) {
  radius = 0.5;
  detail = 3;
  rotation = 0;
  createShape();
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

void Sphere::render(glm::mat4 view, glm::mat4 projection) {
  model = glm::rotate(model, rotation, glm::vec3(0,1,0));
  glm::mat4 mvp = projection * view * model;
  glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  // FIXME: if indice size changes while rendering this will blow up
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
#ifdef DEBUG
  glBindVertexArray(0);
#endif
}

void Sphere::update() {
}

