#include "Universe.hpp"

Universe::Universe(GLuint matrixID, GLuint width, GLuint height) {
  // matrices
  projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
  view = glm::lookAt(glm::vec3(3,3,3), 
		     glm::vec3(0,0,0),
		     glm::vec3(0,1,0));

  spheres.push_back(new Sphere(matrixID, glm::vec3(-2.0, 0.0, 0.0), 0.5));
  spheres.push_back(new Sphere(matrixID, glm::vec3(2.0, 0.0, 0.0), 0.5));
}

void Universe::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (size_t i = 0; i < spheres.size(); i++) {
    spheres[i]->render(view, projection);
  }
}

void Universe::update() {
  for (size_t i = 0; i < spheres.size(); i++) {
    spheres[i]->update();
  }
}

void Universe::handleKey(int key) {
  if (key == GLFW_KEY_LEFT) {
    for (size_t i = 0; i < spheres.size(); i++) {
      spheres[i]->rotation = 0.3;
    }
  } else if (key == GLFW_KEY_RIGHT) {
    for (size_t i = 0; i < spheres.size(); i++) {
      spheres[i]->rotation = -0.3;
    }
  } 
}
