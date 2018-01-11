#include "Program.hpp"
#include <chrono>
#include "util.hpp"
#include "config.hpp"

#ifdef DEBUG
void _post_call_callback_default(const char *name, void *funcptr, int len_args, ...) {
    GLenum error_code;
    error_code = glad_glGetError();

    if (error_code != GL_NO_ERROR) {
        fprintf(stderr, "ERROR %d in %s\n", error_code, name);
    }
}
#endif

Program::Program(std::string name, std::string vertex_shader, std::string tess_control_shader, std::string tess_eval_shader, std::string fragment_shader,
		 std::string config_path, GLuint width, GLuint height, float updateTime): updateTime(updateTime) {
  if(!glfwInit()) {
    throw std::string("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
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
  
  GLuint program = createProgram(vertex_shader.c_str(), tess_control_shader.c_str(), tess_eval_shader.c_str(), fragment_shader.c_str());
  glUseProgram(program);
  
  //glViewport(0, 0, width, height);
  //glClearColor(0.0, 0.0, 1.0, 1.0);

  GLfloat borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // start the universe
  //universe = new Universe(1.0, program, updateTime, width, height);
  universe = readConfig(config_path.c_str(),  program,  updateTime, width, height);

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
