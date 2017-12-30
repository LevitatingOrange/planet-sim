#include "gl_headers.hpp"

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
  GLuint diffuse;
  GLuint diffuseNight;
  GLuint specular;
  GLuint normalMap;
  bool useNight;
  GLuint useNightID;
  bool useSpecular;
  GLuint useSpecularID;
  bool useNormal;
  GLuint useNormalID;
public:
  Texture(GLuint program, const char* diffusePath, const char* diffuseNightPath, const char* specularPath, const char* normalPath);
  void render();
};

#endif
