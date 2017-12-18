#include "Universe.hpp"
#include <cmath>

Universe::Universe(GLuint width, GLuint height, float updateTime):
  eye(glm::vec3(50, 0, -50)), pitch(0), roll(0), yaw(0), angle_modifier(0.01), speed_modifier(0.01), speed(0.0),
  updateTime(updateTime), running(false), time(0.0), pressed_space(false) {
  // matrices
  projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 200.0f);
  
  bodies.push_back(new CelestialBody(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0),
				     100, 4, 5, 0.05, 45));
  bodies.push_back(new CelestialBody(glm::vec3(15.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 100),
				     0.00001, 4, 0.5, -0.1, -10));
  bodies.push_back(new CelestialBody(glm::vec3(-30.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -100),
				     0.00001, 4, 0.5, -0.1, -10));
}

Universe::~Universe() {
  // TODO proper deletion?! (does std::vector get deleted by RAII automatically?)
  //delete bodies;
}

void Universe::render() {
  glm::mat4x4 rot = glm::yawPitchRoll(yaw, pitch, roll);
  view = rot * glm::translate(eye);
  glm::vec4 new_eye = glm::normalize(glm::vec4(0, 0, 1, 0) * rot) * speed;
  eye += glm::vec3(new_eye.x, new_eye.y, new_eye.z);
  //eye = eye + (direction * speed);
  //view = glm::lookAt(eye, 
  //		     glm::vec3(0,0,0),
  //		     glm::vec3(0,1,0));
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (size_t i = 0; i < bodies.size(); i++) {
    bodies[i]->render(view, projection);
  }
}

void Universe::calculate() {
  float dt = updateTime / 10000;
  for (size_t i = 0; i < bodies.size(); i++) {
    auto bi = bodies[i];

    // update position
    bi->position = bi->position + bi->velocity * dt;

    // calculate acceleration
    glm::vec3 force = glm::vec3(0,0,0);
    for (size_t j = 0; j < bodies.size(); j++) {
      auto bj = bodies[j];
      if (i != j) {
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
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    pitch = angle_clip(pitch, -angle_modifier);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    pitch = angle_clip(pitch, angle_modifier);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    yaw = angle_clip(yaw, -angle_modifier);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    yaw = angle_clip(yaw, angle_modifier);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    roll = angle_clip(roll, angle_modifier);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    roll = angle_clip(roll, -angle_modifier);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    speed += speed_modifier;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    speed -= speed_modifier;
  }
}
