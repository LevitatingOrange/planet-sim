#include "Universe.hpp"
#include "gl_headers.hpp"
#ifndef CONFIG_H
#define CONFIG_H
Universe* readConfig(const char* configPath, GLuint program, float updateTime, GLuint width, GLuint height);
#endif
