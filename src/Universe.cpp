#include "Universe.hpp"
#include <cmath>
#include <iostream>

Universe::Universe(double g, double timeScale, double updateTime, GLuint width, GLuint height):
  g(g), timeScale(timeScale), useLocalCamera(false), updateTime(updateTime),
  running(false), showOrbits(false), time(0.0), pressed(0) {
  // matrices
  mainShader = new MainShader();
  orbitShader = new OrbitShader();
  mainShader->use();
  projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.001f, 10000.0f);
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

  size_t numLights = 0;
  
  for (size_t i = 0; i < bodies.size(); i++) {
    if (bodies[i]->isStar) {
      numLights++;
    }
  }
  mainShader->setNumLights(numLights);

  size_t j = 0;
  for (size_t i = 0; i < bodies.size(); i++) {
    if (bodies[i]->isStar) {
      ((Star*) bodies[i])->setLights(camera->eye, j);
      j++;
    }
  }
  for (size_t i = 0; i < bodies.size(); i++) {
    bodies[i]->render(camera->eye);
  }

  // render orbits
  if (showOrbits) {
    orbitShader->use();
    orbitShader->setView(camera->view);
    orbitShader->setProjection(projection);
    orbitShader->setModel(glm::mat4(1.0));
    for (size_t i = 0; i < bodies.size(); i++) {
      bodies[i]->renderOrbit();
    }
  }
}

void Universe::calculate() {
  double dt = updateTime * timeScale;
  for (size_t i = 0; i < bodies.size(); i++) {
    auto bi = bodies[i];

    // update position
    bi->position = bi->position + bi->velocity * dt;

    // calculate acceleration
    glm::dvec3 force = glm::dvec3(0,0,0);
    for (size_t j = 0; j < bodies.size(); j++) {
      auto bj = bodies[j];
      if (i != j) {
    	force += (bj->mass * (bi->position - bj->position)) / 
    	  std::pow(glm::distance(bi->position, bj->position), 3);
      }
    }
    glm::dvec3 acceleration = g * force * -1.0;

    // update velocity
    bi->velocity = bi->velocity + acceleration * dt;
  }
}

void Universe::update() {
  if (useLocalCamera && camera->cameraType == GLOBAL_CAMERA) {
    free(camera);
    camera = new LocalCamera(mainShader, &bodies);
  } else if (!useLocalCamera && camera->cameraType == LOCAL_CAMERA) {
    free(camera);
    camera = new GlobalCamera(mainShader, &bodies);
  }
  
  camera->update();
  if (running) {
    for (size_t i = 0; i < bodies.size(); i++) {
      calculate();
      bodies[i]->update(updateTime * timeScale);
    }
    time += updateTime;
  }
}


void Universe::processKey(int key, bool* val) {
  if (glfwGetKey(window, key) == GLFW_PRESS) {
    if (!pressed[key]) {
      (*val) = !(*val);
    }
    pressed[key] = true;
  }
  if (glfwGetKey(window, key) == GLFW_RELEASE) {
    pressed[key] = false;
  }
}


void Universe::processInput() {
  
  camera->processInput(window);

  processKey(GLFW_KEY_SPACE, &running);
  processKey(GLFW_KEY_O, &showOrbits);
  processKey(GLFW_KEY_L, &useLocalCamera);

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    if (!pressed[GLFW_KEY_ESCAPE]) {
      if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      } else {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
      }
      pressed[GLFW_KEY_ESCAPE]= true;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
    pressed[GLFW_KEY_ESCAPE] = false;
  }
}
