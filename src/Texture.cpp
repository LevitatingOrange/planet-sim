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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

Texture::Texture(GLuint program, const char* diffusePath, const char* diffuseNightPath): useNight(diffuseNightPath != nullptr), use_night_id(glGetUniformLocation(program, "useNightTexture")) {
  generateTexture(diffusePath, &diffuse);
  glUniform1i(glGetUniformLocation(program, "diffuseDayTexture"), 0);
  if (useNight) {
    generateTexture(diffuseNightPath, &diffuseNight);
    glUniform1i(glGetUniformLocation(program, "diffuseNightTexture"), 1);
  }
}

void Texture::render() {
  glUniform1i(use_night_id, useNight);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuse);
  if (useNight) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffuseNight);
  }
}
