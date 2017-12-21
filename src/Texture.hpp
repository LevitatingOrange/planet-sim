#include "gl_headers.hpp"

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
  GLuint diffuse;
public:
  Texture(const char* diffusePath);
  void render();
};

#endif
