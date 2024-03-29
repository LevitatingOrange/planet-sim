#include "Text.hpp"
#include <iostream>

Text::Text(std::string fontName, GLuint width, GLuint height, glm::vec2 position, float scale, unsigned int fontsize, float lineHeight, glm::vec3 textColor):
  shader(new TextShader()), position(position), scale(scale), lineHeight(lineHeight), textColor(textColor) {

  projection = glm::ortho(0.0f, (float) width, 0.0f, (float) height);

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    throw std::string("Could not init FreeType");
  }
  FT_Face face;
  if (FT_New_Face(ft, fontName.c_str(), 0, &face)) {
    throw std::string("Could not load font");
  }

  // TODO DPI not hardcoded!
  FT_Set_Char_Size(face, 0, fontsize*64, 220, 220);

  for (GLubyte c = 0; c < 128; c++) {
    // if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
    if (FT_Load_Char(face, c, FT_LOAD_DEFAULT)) {
      throw std::string("Could not load Glyph");
    }
    if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) {
      throw std::string("Could not render Glyph");
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
		 GL_TEXTURE_2D,
		 0,
		 GL_RGB,
		 (GLuint) face->glyph->bitmap.width,
		 (GLuint) face->glyph->bitmap.rows,
		 0,
		 GL_RED,
		 GL_UNSIGNED_BYTE,
		 face->glyph->bitmap.buffer
		 );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    Character character = {
        texture, 
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        (GLuint) face->glyph->advance.x
    };
    chars.insert(std::pair<GLchar, Character>(c, character));
  }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Text::render() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLfloat y = position.y;
  shader->use();
  shader->setTextColor(textColor);
  shader->setProjection(projection);
  shader->setTextureIDs();
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);
  
  for (size_t i = 0; i < lines.size(); i++) {
    GLfloat x = position.x;
    std::string::const_iterator c;
    for (c = lines[i].begin(); c != lines[i].end(); c++) {
      Character ch = chars[*c];
      
      GLfloat xpos = x + ch.bearing.x * scale;
      GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;
      
      GLfloat w = ch.size.x * scale;
      GLfloat h = ch.size.y * scale;
      // Update VBO for each character
      GLfloat vertices[6][4] = {
	{ xpos,     ypos + h,   0.0, 0.0 },            
	{ xpos,     ypos,       0.0, 1.0 },
	{ xpos + w, ypos,       1.0, 1.0 },

	{ xpos,     ypos + h,   0.0, 0.0 },
	{ xpos + w, ypos,       1.0, 1.0 },
	{ xpos + w, ypos + h,   1.0, 0.0 }           
      };
      // Render glyph texture over quad
      glBindTexture(GL_TEXTURE_2D, ch.textureID);
      // Update content of VBO memory    std::cout << lines[i] << std::endl;
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      // Render quad
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    y += lineHeight;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
}

void Text::setDimensions(GLuint width, GLuint height) {
  projection = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
}

