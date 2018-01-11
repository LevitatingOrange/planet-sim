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

void loadShader(GLuint shader, const char* path) {
  std::string shaderSource = readFile(path);
  const char* c_shaderSource = shaderSource.c_str();

  GLint result = GL_FALSE;
  int logLength;

  // compile vertex shader
  glShaderSource(shader, 1, &c_shaderSource, NULL);
  glCompileShader(shader);
  // check for errors
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 1) {
    std::vector<char> shaderError(logLength);
    glGetShaderInfoLog(shader, logLength, NULL, shaderError.data());
    if (result == GL_TRUE) {
      log(WARNING, std::string(shaderError.data()));
    } else {
      throw std::string("Could not compile shader (") + std::string(path) + std::string("): ") + std::string(shaderError.data());
    }
  }
}


GLuint createProgram(const char *vertex_path, const char* tess_control_path, const char* tess_eval_path, const char* fragment_path) {
  GLint result = GL_FALSE;
  int logLength;
  GLuint program = glCreateProgram();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  loadShader(vertexShader, vertex_path);
  glAttachShader(program, vertexShader);
  glDeleteShader(vertexShader);
  
  if (tess_control_path != nullptr) {
    GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    loadShader(tessControlShader, tess_control_path);
    glAttachShader(program, tessControlShader);
    glDeleteShader(tessControlShader);
  }
  
  if (tess_eval_path != nullptr) {
    GLuint tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    loadShader(tessEvalShader, tess_eval_path);
    glAttachShader(program, tessEvalShader);
    glDeleteShader(tessEvalShader);
  }
  
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  loadShader(fragmentShader, fragment_path);
  glAttachShader(program, fragmentShader);
  glDeleteShader(fragmentShader);

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

  return program;
}
