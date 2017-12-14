#include "Universe.hpp"
#include <cmath>

Universe::Universe(GLuint width, GLuint height, float updateTime): updateTime(updateTime) {
  // matrices
  projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 200.0f);
  view = glm::lookAt(glm::vec3(50,50,50), 
		     glm::vec3(0,0,0),
		     glm::vec3(0,1,0));

  bodies.push_back(new CelestialBody(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0),
				     100, 4, 5, 0.05, 45));
  bodies.push_back(new CelestialBody(glm::vec3(15.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 100),
				     0.001, 4, 0.5, -0.1, -10));
  running = false;
  time = 0;
  pressed_space = false;
}

Universe::~Universe() {
  // TODO proper deletion?! (does std::vector get deleted by RAII automatically?)
  //delete bodies;
}

void Universe::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (size_t i = 0; i < bodies.size(); i++) {
    bodies[i]->render(view, projection);
  }
}

void Universe::calculate() {
  float dt = updateTime / 10000;
  for (size_t i = 0; i < bodies.size(); i++) {
    auto bi = bodies[i];
    //printf("%f %f %f\n", bi->position.x, bi->position.y, bi->position.z);

    // update position
    bi->position = bi->position + bi->velocity * dt;
    //  vprint(bi->position);

    // calculate acceleration
    glm::vec3 force = glm::vec3(0,0,0);
    for (size_t j = 0; j < bodies.size(); j++) {
      auto bj = bodies[j];
      if (i != j) {
	//   	vprint(bi->position);
	//	vprint(bj->position);
	//	printf("%f\n---\n", glm::distance(bi->position, bj->position));
    	force += (bi->mass * bj->mass * (bi->position - bj->position)) / 
    	  ((float) std::pow(glm::distance(bi->position, bj->position), 1.0/3));
      }
    }
    force = force * -1.0f;
    glm::vec3 acceleration = force / bi->mass;

    // update velocity
    bi->velocity = bi->velocity + acceleration * dt;
  }
}

void Universe::update() {
  if (running) {
    for (size_t i = 0; i < bodies.size(); i++) {
      calculate();
      bodies[i]->update();
    }
    time += updateTime;
  }
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
