#include "Program.hpp"
#include <fstream>
#include <sstream>
#include <chrono>

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


Program::Program(std::string name, std::string vertex_shader, std::string fragment_shader, GLuint width, GLuint height, float updateTime): updateTime(updateTime) {
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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
  
  GLuint program = loadShader(vertex_shader.c_str(), fragment_shader.c_str());
  glUseProgram(program);
  
  //glViewport(0, 0, width, height);
  //glClearColor(0.0, 0.0, 1.0, 1.0);

  GLfloat borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // start the universe
  universe = new Universe(program, width, height, updateTime);
}

// http://gameprogrammingpatterns.com/game-loop.html
void Program::startMainLoop() {
  double previous = glfwGetTime() * 1000;
  double lag = 0.0;
  
  do {
    double current = glfwGetTime() * 1000;
    double elapsed = current - previous;
    previous = current;
    lag += elapsed;
    
    glfwPollEvents();
    universe->processInput(window);

    while (lag >= updateTime) {
      universe->update();
      lag -= updateTime;
    }
    // TODO: fix stuttering (see article above)
    universe->render();
    glfwSwapBuffers(window);
  } while(glfwWindowShouldClose(window) == 0);
}


Program::~Program() {
  std::cout << "Program terminated" << std::endl;
}
