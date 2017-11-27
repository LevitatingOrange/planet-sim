static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

void draw_triangle(GLuint vb) {
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);
}

int main(int agrc, char* argv[]) {
 

    // VAO
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // VBO
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    draw_triangle(vertexbuffer);
}
