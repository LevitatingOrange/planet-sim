#include "Sphere.hpp"
#include <iostream>

Vertex nv(glm::vec3 position, glm::vec2 texture) {
  return Vertex {.position = position, .texture = texture};
}

void Sphere::subdivide(Vertex a, Vertex b, Vertex c, int depth) {
  if (depth == 0) {
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
  } else {
    Vertex m1 = nv(midpoint(a.position, b.position), midpoint(a.texture, b.texture));
    Vertex m2 = nv(midpoint(a.position, c.position), midpoint(a.texture, c.texture));
    Vertex m3 = nv(midpoint(b.position, c.position), midpoint(b.texture, c.texture));
    
    subdivide(a, m1, m2, depth-1);
    subdivide(m1, b, m3, depth-1);
    subdivide(m2, m3, c, depth-1);
    subdivide(m1, m2, m3, depth-1);
  }
}


void Sphere::reduce() {
  std::vector<Vertex> new_vertices;
  
  for (size_t i = 0; i < vertices.size(); i++) {
    if (indices[i] == i) {
      Vertex curr = vertices[i];
      new_vertices.push_back(curr);
    
      for (size_t j = 0; j < vertices.size(); j++) {
        Vertex dup = vertices[j];
	if (glm::distance(curr.position, dup.position) < 0.001 && glm::distance(curr.texture, dup.texture) < 0.001) {
	  indices[j] = new_vertices.size() - 1;
	}
      }
    }
  }
  vertices = new_vertices;
}

void Sphere::generateMesh() {
  float r = radius;
  // generate octahedron
  // top view (order of triangles):
  //      3
  //   +-----+
  //   |     |
  // 4 |     | 2
  //   |     |
  //   +-----+
  //      1
  // up
  subdivide(nv(glm::vec3(-r, 0, r), glm::vec2(0.0, 0.5)), nv(glm::vec3(0, r, 0), glm::vec2(0.125, 1)), nv(glm::vec3(r, 0, r), glm::vec2(0.25, 0.5)), detail);
  subdivide(nv(glm::vec3(r, 0, r), glm::vec2(0.25, 0.5)), nv(glm::vec3(0, r, 0), glm::vec2(0.375, 1)), nv(glm::vec3(r, 0, -r), glm::vec2(0.5, 0.5)), detail);
  subdivide(nv(glm::vec3(r, 0, -r), glm::vec2(0.5, 0.5)), nv(glm::vec3(0, r, 0), glm::vec2(0.625, 1)), nv(glm::vec3(-r, 0, -r), glm::vec2(0.75, 0.5)), detail);
  subdivide(nv(glm::vec3(-r, 0, -r), glm::vec2(0.75, 0.5)), nv(glm::vec3(0, r, 0), glm::vec2(0.875, 1)), nv(glm::vec3(-r, 0, r), glm::vec2(1, 0.5)), detail);

  // down
  subdivide(nv(glm::vec3(-r, 0, r), glm::vec2(0.0, 0.5)), nv(glm::vec3(0, -r, 0), glm::vec2(0.125, 0)), nv(glm::vec3(r, 0, r), glm::vec2(0.25, 0.5)), detail);
  subdivide(nv(glm::vec3(r, 0, r), glm::vec2(0.25, 0.5)), nv(glm::vec3(0, -r, 0), glm::vec2(0.375, 0)), nv(glm::vec3(r, 0, -r), glm::vec2(0.5, 0.5)), detail);
  subdivide(nv(glm::vec3(r, 0, -r), glm::vec2(0.5, 0.5)), nv(glm::vec3(0, -r, 0), glm::vec2(0.625, 0)), nv(glm::vec3(-r, 0, -r), glm::vec2(0.75, 0.5)), detail);
  subdivide(nv(glm::vec3(-r, 0, -r), glm::vec2(0.75, 0.5)), nv(glm::vec3(0, -r, 0), glm::vec2(0.875, 0)), nv(glm::vec3(-r, 0, r), glm::vec2(1, 0.5)), detail);
}

void Sphere::spherify() {
  for (size_t i = 0; i < vertices.size(); i++) {
    vertices[i].position = normalize(glm::vec3(0,0,0), vertices[i].position, radius);
  }
}

void Sphere::completeVertices() {
  for (size_t i = 0; i < vertices.size(); i++) {
    vertices[i].normal = vertices[i].position / radius;
    vertices[i].color = color;
  }
}

void Sphere::createShape() {
  vertices.clear();
  indices.clear();
  generateMesh();
  //reduce();
  spherify();
  completeVertices();
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
  
#ifdef DEBUG
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

Sphere::Sphere(GLuint program, glm::vec3 color, unsigned int detail, float radius, float rotation_angle, float obliquity) :
  model_id(glGetUniformLocation(program, "model")), color(color), detail(detail), rotation(glm::rotate(glm::mat4(1.0f), obliquity, glm::vec3(0,0,1))),
  translation(glm::mat4(1.0f)), old_position(glm::vec3(0,0,0)), radius(radius), rotation_angle(rotation_angle), obliquity(obliquity) {
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
#ifdef DEBUG
  glBindVertexArray(0);
#endif
}

void Sphere::update(glm::vec3 position) {
  translation = glm::translate(translation, old_position - position);
  rotation = glm::rotate(rotation, rotation_angle, glm::vec3(0,1,0));
  old_position = position;
}

