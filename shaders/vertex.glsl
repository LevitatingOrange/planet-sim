#version 400 core

layout(location = 0) in vec3 position;

out vec3 vertexPosition;

void main() {
  vertexPosition = position;
}
