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
    float latitude = acos(vectors[i].z / radius) / M_PI;
    float longitude = (atan2(vectors[i].y , vectors[i].x) + M_PI) / (2 * M_PI);
    vertices.push_back(Vertex {vectors[i], vectors[i] / radius, color, glm::vec2(longitude, latitude)});
  }
}

void Sphere::createShape() {
  vectors.clear();
  vertices.clear();
  indices.clear();
  generateMesh();
  //reduce();
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
  glEnableVertexAttribArray(VERTEX_ID);
  glVertexAttribPointer(VERTEX_ID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(NORMAL_ID);
  glVertexAttribPointer(NORMAL_ID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));
  glEnableVertexAttribArray(COLOR_ID);
  glVertexAttribPointer(COLOR_ID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 6));
  glEnableVertexAttribArray(TEXTURE_ID);
  glVertexAttribPointer(TEXTURE_ID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 9));
  // EBO
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

Sphere::Sphere(GLuint program, glm::vec3 color, unsigned int detail, float radius, float rotation_angle, float obliquity) :
  model_id(glGetUniformLocation(program, "model")), color(color), detail(detail), rotation(glm::rotate(glm::mat4(1.0f), (float) M_PI/2, glm::vec3(1,0,0))),
  translation(glm::mat4(1.0f)), old_position(glm::vec3(0,0,0)), radius(radius), rotation_angle(rotation_angle), obliquity(obliquity) {
  rotation = glm::rotate(rotation, obliquity, glm::vec3(0,0,1));
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

void Sphere::render() {
  glm::mat4 model = translation * rotation;
  glUniformMatrix4fv(model_id, 1, GL_FALSE, &model[0][0]);

  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  // FIXME: if indice size changes while rendering this will blow up
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void Sphere::update(glm::vec3 position) {
  translation = glm::translate(translation, old_position - position);
  rotation = glm::rotate(rotation, rotation_angle, glm::vec3(0,0,1));
  old_position = position;
}

