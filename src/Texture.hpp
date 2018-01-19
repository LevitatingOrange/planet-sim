#include "headers.h"

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
  MainShader* mainShader;
  
  GLuint diffuse;
  GLuint diffuseNight;
  GLuint specular;
  GLuint normalMap;
  bool useNight;
  bool useSpecular;
  bool useNormal;
public:
  Texture(MainShader* mainShader, const char* diffusePath, const char* diffuseNightPath, const char* specularPath, const char* normalPath);
  ~Texture();
  void render();
};

#endif
