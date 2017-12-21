#version 330 core

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec3 fragmentColor;

void main() {
  gl_Position = projection * view * model * vec4(vertexPosition, 1);
  fragmentPosition = vec3(model * vec4(vertexPosition, 1.0));
  fragmentNormal = vec3(model * vec4(vertexNormal, 0.0));
  fragmentColor = vertexColor;
}
