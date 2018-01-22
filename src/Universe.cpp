#include "Universe.hpp"
#include <cmath>
#include <iostream>

Universe::Universe(double g, double timeScale, double updateTime, GLuint width, GLuint height):
  g(g), timeScale(timeScale), updateTime(updateTime), running(false),
  time(0.0), pressed_space(false), pressed_escape(false) {
  // matrices
  mainShader = new MainShader();
  orbitShader = new OrbitShader();
  mainShader->use();
  projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 200.0f);
  camera = new GlobalCamera(mainShader, &bodies);
}

Universe::~Universe() {
  // TODO proper deletion?! (does std::vector get deleted by RAII automatically?)
  //delete bodies;
}

void Universe::render() {
  
  mainShader->use();
  camera->render();
  mainShader->setProjection(projection);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (size_t i = 0; i < bodies.size(); i++) {
    bodies[i]->render(camera->eye);
  }

  // render orbits
  orbitShader->use();
  orbitShader->setView(camera->view);
  orbitShader->setProjection(projection);
  orbitShader->setModel(glm::mat4(1.0));
  for (size_t i = 0; i < bodies.size(); i++) {
     bodies[i]->renderOrbit();
  }
}

void Universe::calculate() {
  double dt = updateTime * timeScale;
  for (size_t i = 0; i < bodies.size(); i++) {
    auto bi = bodies[i];

    // update position
    bi->position = bi->position + bi->velocity * dt;

    // calculate acceleration
    //std::cout << "Object " << i << ": " << std::endl;
    glm::dvec3 force = glm::dvec3(0,0,0);
    for (size_t j = 0; j < bodies.size(); j++) {
      auto bj = bodies[j];
      if (i != j) {
    	force += (bj->mass * (bi->position - bj->position)) / 
    	  std::pow(glm::distance(bi->position, bj->position), 3);
      }
      //std::cout << "    to Object " << j << " " << std::pow(glm::distance(bi->position, bj->position), 3) << std::endl;
    }
    glm::dvec3 acceleration = g * force * -1.0;

    // update velocity
    bi->velocity = bi->velocity + acceleration * dt;
  }
}

void Universe::update() {
  if (running) {
    for (size_t i = 0; i < bodies.size(); i++) {
      calculate();
      bodies[i]->update(updateTime * timeScale);
    }
    time += updateTime;
  }
}



void Universe::processInput(GLFWwindow* window) {
  
  camera->processInput(window);

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (!pressed_space) {
      running = !running;
    }
    pressed_space = true;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    pressed_space = false;
  }

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    if (!pressed_escape) {
      if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      } else {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
      }
      pressed_escape = true;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
    pressed_escape = false;
  }
}
