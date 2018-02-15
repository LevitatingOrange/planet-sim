#include "headers.h"
#include <string>
#include <vector>
#include <map>

#ifndef TEXT_HPP
#define TEXT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
  GLuint textureID;
  glm::ivec2 size;
  glm::ivec2 bearing;
  GLuint advance;
};
class Text {
private:
  TextShader* shader;
  std::map<GLchar, Character> chars;
  GLuint VAO;
  GLuint VBO;
  glm::mat4 projection;
public:
  std::vector<std::string> lines;
  glm::vec2 position;
  float scale;
  float lineHeight;
  glm::vec3 textColor;
  
  Text(std::string fontName, GLuint width, GLuint height, glm::vec2 position, float scale, unsigned int fontsize, float lineHeight, glm::vec3 textColor);
  ~Text();
  
  void render();
  void setDimensions(GLuint width, GLuint height);
};
#endif
