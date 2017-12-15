#include "Universe.hpp"

Universe::Universe(GLuint width, GLuint height, double updateTime): updateTime(updateTime) {
  // matrices
  projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
  view = glm::lookAt(glm::vec3(50,50,50), 
		     glm::vec3(0,0,0),
		     glm::vec3(0,1,0));

  spheres.push_back(new Sphere(4, glm::vec3(-2.0, 0.0, 0.0), 0.5, 0.2, 0.4));
  spheres.push_back(new Sphere(4, glm::vec3(2.0, 0.0, 0.0), 0.5, -0.1, 0.2));
  running = true;
  time = 0;
  pressed_space = false;
}

void Universe::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (size_t i = 0; i < spheres.size(); i++) {
    spheres[i]->render(view, projection);
  }
}

void Universe::update() {
  if (running) {
    for (size_t i = 0; i < spheres.size(); i++) {
      spheres[i]->update();
    }
  }
  time += updateTime;
}

void Universe::processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (!pressed_space) {
      running = !running;
    }
    pressed_space = true;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    pressed_space = false;
  }
}
