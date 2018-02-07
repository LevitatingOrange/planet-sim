#include "Program.hpp"
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

void resize_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  Universe* universe = (Universe*) glfwGetWindowUserPointer(window);
  if (universe != NULL) {
    universe->setDimensions(width, height);
  }
}

Program::Program(std::string name, std::string config_path, GLuint width, GLuint height, float updateTime): updateTime(updateTime) {
  if(!glfwInit()) {
    throw std::string("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_SAMPLES, 8); // antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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


  //glViewport(0, 0, width, height);
  //glClearColor(0.0, 0.0, 1.0, 1.0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_FRONT);

  // camera correction
  glEnable(GL_FRAMEBUFFER_SRGB); 

  //fpsDisplay = new Text(std::string("/System/Library/Fonts/Menlo.ttc"), width, height, glm::vec2(width/2,height/2), 1, 1, glm::vec3(1.0, 1.0, 1.0));
  //fpsDisplay->lines.push_back("FPS: ");
  
  // start the universe
  //universe = new Universe(1.0, program, updateTime, width, height);
  universe = readConfig(config_path.c_str(), updateTime, width, height);
  universe->window = window;

  glfwSetWindowUserPointer(window, universe);
  
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
    universe->processInput();

    while (lag >= updateTime) {
      universe->update();
      lag -= updateTime;
    }
    // TODO: fix stuttering (see article above)
    //fpsDisplay->render();
    universe->render();
    glfwSwapBuffers(window);
  } while(glfwWindowShouldClose(window) == 0);
}


Program::~Program() {
  std::cout << "Program terminated" << std::endl;
}
