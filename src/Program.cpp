#include "Program.hpp"
#include <fstream>
#include <sstream>

#ifdef DEBUG
void _post_call_callback_default(const char *name, void *funcptr, int len_args, ...) {
    GLenum error_code;
    error_code = glad_glGetError();

    if (error_code != GL_NO_ERROR) {
        fprintf(stderr, "ERROR %d in %s\n", error_code, name);
    }
}
#endif

std::string readFile(const char* filePath) {
  std::ifstream fileStream(filePath, std::ios::in);

  if (!fileStream.is_open()) {
    throw std::string("Could not read file");
  }

  std::stringstream buffer;
  buffer << fileStream.rdbuf();
  buffer << "\0";
  fileStream.close();
  
  return buffer.str();
}

GLuint loadShader(const char *vertex_path, const char* fragment_path) {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  const char* vertexShaderSource = readFile(vertex_path).c_str();
  const char* fragmentShaderSource = readFile(fragment_path).c_str();

  GLint result = GL_FALSE;
  int logLength;

  // compile vertex shader
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for errors
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 1) {
    std::vector<char> vertexShaderError(logLength);
    glGetShaderInfoLog(vertexShader, logLength, NULL, vertexShaderError.data());
    throw std::string("Could not compile vertex shader: ") + std::string(vertexShaderError.data());
  }

  // compile fragment shader
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 1) {
    std::vector<char> fragmentShaderError(logLength);
    glGetShaderInfoLog(fragmentShader, logLength, NULL, fragmentShaderError.data());
    throw std::string("Could not compile fragment shader: ")  + std::string(fragmentShaderError.data());
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
    throw std::string("Could not link program: ") + std::string(programError.data());
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}


Program::Program(std::string name, std::string vertex_shader, std::string fragment_shader, GLuint width, GLuint height) {
  if(!glfwInit()) {
    throw std::string("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
  if (window == NULL) {
    glfwTerminate();
    throw std::string("Failed to open GLFW window");
  }

  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

#ifdef DEBUG
  glad_set_post_callback(&_post_call_callback_default);
#endif
  
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  GLuint program = loadShader(vertex_shader.c_str(), fragment_shader.c_str());
  glUseProgram(program);
  
  //glViewport(0, 0, width, height);
  //glClearColor(0.0, 0.0, 1.0, 1.0);

  // matrices
  GLuint matrixID = glGetUniformLocation(program, "MVP");
  projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
  view = glm::lookAt(glm::vec3(4,3,3), 
		     glm::vec3(0,0,0),
		     glm::vec3(0,1,0));
  // models
  spheres.push_back(new Sphere(matrixID));
}

void Program::startMainLoop() {
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    spheres[0]->render(view, projection);
    glfwSwapBuffers(window);
    glfwPollEvents();

  } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

Program::~Program() {
  std::cout << "Program terminated" << std::endl;
}
