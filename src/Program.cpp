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
  std::cout << "window resized: width: " << width << " height: "
	    << height  << std::endl;
  glViewport(0, 0, width, height);
  Program* program = (Program*) glfwGetWindowUserPointer(window);
  if (program != NULL) {
    program->universe->setDimensions(width, height);
    program->fpsDisplay->setDimensions(width, height);
    program->fpsDisplay->position = glm::vec2(width-200.0f, height-50.0f);
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

  GLint fwidth;
  GLint fheight;
  glfwGetFramebufferSize(window, &fwidth, &fheight);

  fpsDisplay = new Text(std::string("/System/Library/Fonts/Helvetica.ttc"), fwidth, fheight, glm::vec2(fwidth-200.0f, fheight-50.0f), 1.0, 14, 64, glm::vec3(1.0, 1.0, 1.0));
  fpsDisplay->lines.push_back("FPS: ");
  
  // start the universe
  //universe = new Universe(1.0, program, updateTime, width, height);
  universe = readConfig(config_path.c_str(), updateTime, fwidth, fheight);
  universe->window = window;

  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, resize_callback);
}

// http://gameprogrammingpatterns.com/game-loop.html
void Program::startMainLoop() {
  double previous = glfwGetTime() * 1000;
  double lag = 0.0;
  int fps = 0;
  
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
    universe->render();

    fps = (fps * FPS_DISPLAY_SMOOTHING) + (1000/elapsed * (1.0 - FPS_DISPLAY_SMOOTHING));
    fpsDisplay->lines[0] = std::string("FPS: ") + std::to_string(fps);
    fpsDisplay->render();
    glfwSwapBuffers(window);
  } while(glfwWindowShouldClose(window) == 0);
}


Program::~Program() {
  std::cout << "Program terminated" << std::endl;
}
