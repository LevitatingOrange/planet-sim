#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <iostream>
#include <chrono>

void generateTexture(const char* path, GLuint* texture) {
  using namespace std::chrono;
  stbi_set_flip_vertically_on_load(true);
  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  int width, height, nrChannels;
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::string("Failed to load texture ") + std::string(path);
  }
  high_resolution_clock::time_point t3 = high_resolution_clock::now();
  std::cout << "Time for image load " << duration_cast<microseconds>(t2-t1).count();
  std::cout <<", time for texture uploading and mipmap generation: " << duration_cast<microseconds>(t3 - t2).count() << std::endl;
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
