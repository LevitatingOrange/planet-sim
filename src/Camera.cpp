#include "Camera.hpp"
#include "iostream"

Camera::Camera(MainShader* mainShader, std::vector<CelestialBody*>* bodies, CameraType cameraType):
  mainShader(mainShader), bodies(bodies), cameraType(cameraType),
  eye(glm::vec3(-2, 2, 5)) {
}

GlobalCamera::GlobalCamera(MainShader* mainShader, std::vector<CelestialBody*>* bodies):
  Camera(mainShader, bodies, GLOBAL_CAMERA),
  sensitivity(0.05), lastX(0), lastY(0),
  direction(0, -1, 0),
  camera_up(glm::vec3(0, 0, 1)),
  yaw(-90), pitch(0), speed_modifier(0.001),
  speed(0.1), up(glm::vec3(0, 1, 0)), pressed_r(false) {
}

void GlobalCamera::render() {
  camera_up = glm::normalize(glm::cross(direction, glm::normalize(glm::cross(up, direction))));
  view = glm::lookAt(eye, eye + direction, camera_up);
  mainShader->setView(view);
  mainShader->setViewPosition(eye);
}
void GlobalCamera::processInput(GLFWwindow* window) {
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  float dx = (x - lastX) * sensitivity;
  float dy = (lastY - y) * sensitivity;
  lastX = x;
  lastY = y;
  
  yaw += dx;
  pitch += dy;

  // prevent gimbal lock
  if (pitch >= 90.0) {
    pitch = 89;
  } else if (pitch <= -90) {
    pitch = -89;
  }
  direction = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
  				       sin(glm::radians(pitch)),
  				       sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
  // TODO: fix delta time speeding
  
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    //std::cout << glm::orientedAngle(direction, view_direction) << std::endl;
  }

  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && bodies->size() >= 1) {
    eye = (*bodies)[0]->position * (*bodies)[0]->physicsScale;
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && bodies->size() >= 2) {
    eye = (*bodies)[1]->position * (*bodies)[1]->physicsScale;
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && bodies->size() >= 3) {
    eye = (*bodies)[2]->position * (*bodies)[2]->physicsScale;
  }
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && bodies->size() >= 4) {
    eye = (*bodies)[3]->position * (*bodies)[3]->physicsScale;
  }
  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && bodies->size() >= 5) {
    eye = (*bodies)[4]->position * (*bodies)[4]->physicsScale;
  }
  if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && bodies->size() >= 6) {
    eye = (*bodies)[5]->position * (*bodies)[5]->physicsScale;
  }
  if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && bodies->size() >= 7) {
    eye = (*bodies)[6]->position * (*bodies)[6]->physicsScale;
  }
  if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && bodies->size() >= 8) {
    eye = (*bodies)[7]->position * (*bodies)[7]->physicsScale;
  }
  if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && bodies->size() >= 9) {
    eye = (*bodies)[8]->position * (*bodies)[8]->physicsScale;
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
}

void GlobalCamera::update() {
  return;
}

LocalCamera::LocalCamera(MainShader* mainShader, std::vector<CelestialBody*>* bodies):
  Camera(mainShader, bodies, LOCAL_CAMERA), focusedBody(0), centerBody(0), radius(10), speed(0.1), speed_modifier(0.01) {
}

void LocalCamera::render() {
  glm::vec3 center = (*bodies)[focusedBody]->position * (*bodies)[focusedBody]->physicsScale;
  glm::vec3 direction = eye - center;
  glm::vec3 camera_up = glm::normalize(glm::cross(direction, glm::normalize(glm::cross(glm::vec3(0, 0, -1), direction))));
  view = glm::lookAt(eye, center, camera_up);  
  mainShader->setView(view);
  mainShader->setViewPosition(eye);
}

void LocalCamera::update() {
  glm::vec3 center = (*bodies)[focusedBody]->position * (*bodies)[focusedBody]->physicsScale;
  glm::vec3 newEye = (*bodies)[centerBody]->position * (*bodies)[focusedBody]->physicsScale;
  eye = newEye + radius * glm::normalize(newEye - center); 
}

void LocalCamera::processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    radius += speed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    radius -= speed;
  }
  
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && bodies->size() >= 1) {
      centerBody = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && bodies->size() >= 2) {
      centerBody = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && bodies->size() >= 3) {
      centerBody = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && bodies->size() >= 4) {
      centerBody = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && bodies->size() >= 5) {
      centerBody = 4;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && bodies->size() >= 6) {
      centerBody = 5;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && bodies->size() >= 7) {
      centerBody = 6;
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && bodies->size() >= 8) {
      centerBody = 7;
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && bodies->size() >= 9) {
      centerBody = 8;
    }
  } else {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && bodies->size() >= 1) {
      focusedBody = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && bodies->size() >= 2) {
      focusedBody = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && bodies->size() >= 3) {
      focusedBody = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && bodies->size() >= 4) {
      focusedBody = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && bodies->size() >= 5) {
      focusedBody = 4;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && bodies->size() >= 6) {
      focusedBody = 5;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && bodies->size() >= 7) {
      focusedBody = 6;
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && bodies->size() >= 8) {
      focusedBody = 7;
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && bodies->size() >= 9) {
      focusedBody = 8;
    }
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
}


