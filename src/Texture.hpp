#include "gl_headers.hpp"

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
  GLuint diffuse;
  GLuint diffuseNight;
  bool useNight;
  GLuint use_night_id;
public:
  Texture(GLuint program, const char* diffusePath, const char* diffuseNightPath);
  void render();
};

#endif
