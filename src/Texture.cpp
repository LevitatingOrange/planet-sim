#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>

void generateTexture(const char* path, GLuint* texture) {
  stbi_set_flip_vertically_on_load(true);
  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::string("Failed to load texture ") + std::string(path);
  }
  stbi_image_free(data);
}

Texture::Texture(MainShader* mainShader, const char* diffusePath, const char* diffuseNightPath, const char* specularPath, const char* normalPath):
  mainShader(mainShader), useNight(diffuseNightPath != nullptr),
  useSpecular(specularPath != nullptr), useNormal(normalPath != nullptr) {
  generateTexture(diffusePath, &diffuse);
  if (useNight) {
    generateTexture(diffuseNightPath, &diffuseNight);
  }
  if (useSpecular) {
    generateTexture(specularPath, &specular);
  }
  if (useNormal) {
    generateTexture(normalPath, &normalMap);
  }
}

Texture::~Texture() {
  glDeleteTextures(4, &diffuse);
}

void Texture::render() {
  mainShader->setTextureIDs(useNight, useSpecular, useNormal);
  mainShader->setTextureSettings(useNight, useSpecular, useNormal);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuse);
  if (useNight) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffuseNight);
  }
  if (useSpecular) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specular);
  }
  if (useNormal) {
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalMap);
  }
}
