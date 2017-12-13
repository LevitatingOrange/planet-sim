#include "Sphere.hpp"
#include <iostream>

void Sphere::subdivide(glm::vec3 a, glm::vec3 b, glm::vec3 c, int depth) {
  if (depth == 0) {
    indices.push_back(vectors.size() - 1);
    indices.push_back(vectors.size());
    indices.push_back(vectors.size() + 1);
    vectors.push_back(a);
    vectors.push_back(b);
    vectors.push_back(c);
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

 outer: for (size_t i = 0; i < vectors.size(); i++) {
    glm::vec3 curr = vectors[i];
    for (size_t j = 0; j < new_vectors.size(); j++) {
      if (vequal(curr, new_vectors[j])) {
	goto outer;
      }
    }
    
    for (size_t j = 0; j < vectors.size(); j++) {
      if (i != j) {
	glm::vec3 dup = vectors[j];
	if (vequal(curr, dup)) {
	  indices.push_back(i);
	}
      }
    }
    new_vectors.push_back(curr);
  }
  vectors = new_vectors;
}

void Sphere::generateMesh() {
  subdivide(glm::vec3(radius, 0, -radius), glm::vec3(0, radius, 0), glm::vec3(radius, 0, radius), detail);
  subdivide(glm::vec3(radius, 0, -radius), glm::vec3(0, -radius, 0), glm::vec3(radius, 0, radius), detail);
  subdivide(glm::vec3(radius, 0, -radius), glm::vec3(0, radius, 0), glm::vec3(-radius, 0, radius), detail);
  subdivide(glm::vec3(radius, 0, -radius), glm::vec3(0, -radius, 0), glm::vec3(-radius, 0, radius), detail);
  subdivide(glm::vec3(-radius, 0, radius), glm::vec3(0, radius, 0), glm::vec3(radius, 0, -radius), detail);
  subdivide(glm::vec3(-radius, 0, radius), glm::vec3(0, -radius, 0), glm::vec3(radius, 0, -radius), detail);
  subdivide(glm::vec3(-radius, 0, radius), glm::vec3(0, radius, 0), glm::vec3(radius, 0, radius), detail);
  subdivide(glm::vec3(-radius, 0, radius), glm::vec3(0, -radius, 0), glm::vec3(radius, 0, radius), detail);
  // vertices.push_back(Vertex {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0, 0.0)});
  // vertices.push_back(Vertex {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0, 1.0f, 0.0)});
  // vertices.push_back(Vertex {glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0f)});

  // indices = {
  //  0, 1, 2
  // };
}

void Sphere::generateVertices() {
  for (size_t i = 0; i < vectors.size(); i++) {
    vertices.push_back(Vertex {vectors[i], glm::vec3(1.0, 1.0, 1.0)});
  }
}

Sphere::Sphere(GLuint matrixID) : matrixID(matrixID) {
  // auto s = subdivide(
  // 		     glm::vec3(-1.0f, -1.0f, 0.0f),
  // 		     glm::vec3(1.0, 0.0, 0.0),
  // 		     glm::vec3(1.0f, -1.0f, 0.0f),
  // 		     4);
  //generateOctahedron();
  radius = 0.7;
  detail = 0;
  generateMesh();
  //reduce();
  generateVertices();

  // for (size_t i = 0; i < vectors.size(); i++) {
  //   printf("%f %f %f\n", vectors[i].x, vectors[i].y, vectors[i].z);
  // }
  initGL();
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

Sphere::~Sphere() {
  if (vertexArray != 0) {
    glDeleteVertexArrays(1, &vertexArray);
  }
  if (vertexBuffer != 0) {
    glDeleteBuffers(1, &vertexBuffer);
  }
}

void Sphere::render(glm::mat4 view, glm::mat4 projection, float angleX) {
  model = glm::rotate(model, angleX, glm::vec3(1,0,0));
  
  glm::mat4 mvp = projection * view * model;
  glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  // FIXME: if indice size changes while rendering this will blow up
  glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, (void*)0);
#ifdef DEBUG
  glBindVertexArray(0);
#endif
}
