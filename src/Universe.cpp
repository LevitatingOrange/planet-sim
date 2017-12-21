#include "Universe.hpp"
#include "Star.hpp"
#include "CelestialBody.hpp"
#include <cmath>
#include <iostream>

Universe::Universe(GLuint program, GLuint width, GLuint height, float updateTime):
  projection_id(glGetUniformLocation(program, "projection")), view_id(glGetUniformLocation(program, "view")),
  view_position_id(glGetUniformLocation(program, "view_position")),
  g(1), sensitivity(0.05), lastX(width/2), lastY(height/2), eye(glm::vec3(0, 0, 100)),
  direction(0, 0, -1), camera_up(glm::vec3(0, 1, 0)), yaw(-90), pitch(0), speed_modifier(0.01), speed(1), up(glm::vec3(0, 1, 0)),
  updateTime(updateTime), running(false), time(0.0), pressed_space(false), pressed_escape(false), pressed_r(false), width(width), height(height) {
  // matrices
  projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 200.0f);
  
  bodies.push_back(new Star(program, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0),
			    100, glm::vec3(1.000, 1.000, 1.000), 8, 5, 0.05, 45,
			    Material {glm::vec3(1.3), glm::vec3(0.0), glm::vec3(0.0), 0, false},
			    Light {glm::vec3(1.0),  glm::vec3(1.0), glm::vec3(1.0)}, new Texture(program, "./resources/sun_diffuse4.jpg", nullptr)));
  bodies.push_back(new CelestialBody(program, glm::vec3(15.0, 4.0, 0.0), glm::vec3(0.0, 0.0, 100),
				     0.00001, glm::vec3(1.000, 1.000, 1.000), 8, 0.5, -0.1, -10,
				     Material {glm::vec3(0.1), glm::vec3(0.8), glm::vec3(0.4), 10, false},  new Texture(program, "./resources/earth_diffuse_day.jpg", nullptr)));
}

Universe::~Universe() {
  // TODO proper deletion?! (does std::vector get deleted by RAII automatically?)
  //delete bodies;
}

void Universe::render() {
  camera_up = glm::normalize(glm::cross(direction, glm::normalize(glm::cross(up, direction))));
  view = glm::lookAt(eye, eye + direction, camera_up);
  
  glUniformMatrix4fv(projection_id, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(view_id, 1, GL_FALSE, &view[0][0]);
  glUniform3fv(view_position_id, 1, (GLfloat*) &eye);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (size_t i = 0; i < bodies.size(); i++) {
    bodies[i]->render();
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
    	force += (g * bi->mass * bj->mass * (bi->position - bj->position)) / 
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
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  float dx = (x - lastX) * sensitivity;
  float dy = (lastY - y) * sensitivity;
  lastX = x;
  lastY = y;
  
  yaw += dx;
  pitch += dy;

  // prevent gimbal lock
  if (pitch == 90.0) {
    pitch = 89;
  } else if (pitch == 90) {
    pitch = -89;
  }
  direction = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
  				       sin(glm::radians(pitch)),
  				       sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
  // TODO: fix delta time speeding
  
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    //std::cout << glm::orientedAngle(direction, view_direction) << std::endl;
  }
  
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
    eye += direction * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    eye -= direction * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    eye -= glm::normalize(glm::cross(direction, camera_up)) * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    eye += glm::normalize(glm::cross(direction, camera_up)) * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    eye += up * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    eye -= up * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
    if (!pressed_r) {
      if (yaw < 180) {
	yaw += 180;
      } else {
	yaw -= 180;
      }
    }
    pressed_r = true;
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
    pressed_r = false;
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    speed += speed_modifier;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    speed -= speed_modifier;
    if (speed < 0) {
      speed = 0;
    }
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
