#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

void log(ErrorLevel lvl, std::string msg) {
  switch (lvl) {
  case INFO:
    std::cout << "[Program][INFO]: ";
      break;
  case WARNING:
    std::cout << "[Program][WARNING]: ";
    break;
  case ERROR:
    std::cout << "[Program][ERROR]: ";
    break;
  }
  std::cout << msg << std::endl;
}

void vprint(glm::vec3 vector) {
  std::cout << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
}

void vdprint(glm::dvec3 vector) {
  std::cout << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
}

glm::vec3 normalize(glm::vec3 a, glm::vec3 b, float length) {
  return a + ((b - a) * length / glm::distance(a, b));
}

float angle_clip(float angle, float increase) {
  if (increase > 0) {
    if (angle >= 2 * M_PI) {
      return increase;
    } else {
      return angle + increase;
    }
  }
  else {
    if (angle <= 0) {
      return 2 * M_PI + increase;
    } else {
      return angle + increase;
    }
  }
}

std::string readFile(const char* filePath) {
  std::ifstream fileStream(filePath, std::ios::in);

  if (!fileStream.is_open()) {
    throw std::string("Could not read file");
  }

  std::string content( (std::istreambuf_iterator<char>(fileStream) ),
                       (std::istreambuf_iterator<char>()    ) );
  
  return content;
}

GLuint loadShader(const char *vertex_path, const char* fragment_path) {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  std::string vertexShaderSource = readFile(vertex_path);
  std::string fragmentShaderSource = readFile(fragment_path);

  const char* c_vertexShaderSource = vertexShaderSource.c_str();
  const char* c_fragmentShaderSource = fragmentShaderSource.c_str();

  GLint result = GL_FALSE;
  int logLength;

  // compile vertex shader
  glShaderSource(vertexShader, 1, &c_vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for errors
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 1) {
    std::vector<char> vertexShaderError(logLength);
    glGetShaderInfoLog(vertexShader, logLength, NULL, vertexShaderError.data());
    if (result == GL_TRUE) {
      log(WARNING, std::string(vertexShaderError.data()));
    } else {
      throw std::string("Could not compile vertex shader: ") + std::string(vertexShaderError.data());
    }
  }

  // compile fragment shader
  glShaderSource(fragmentShader, 1, &c_fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 1) {
    std::vector<char> fragmentShaderError(logLength);
    glGetShaderInfoLog(fragmentShader, logLength, NULL, fragmentShaderError.data());
    if (result == GL_TRUE) {
      log(WARNING, std::string(fragmentShaderError.data()));
    } else {
      throw std::string("Could not compile fragment shader: ") + std::string(fragmentShaderError.data());
    }
  }

  // create program and link shaders
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  // check for errors
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 1) {
    std::vector<char> programError(logLength);
    glGetProgramInfoLog(program, logLength, NULL, programError.data());
    if (result == GL_TRUE) {
      log(WARNING, std::string(programError.data()));
    } else {
      throw std::string("Could not link program: ") + std::string(programError.data());

    }
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}
