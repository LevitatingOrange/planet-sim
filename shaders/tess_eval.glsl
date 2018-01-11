#version 400 core

#define M_PI 3.1415926535897932384626433832795

layout(triangles, equal_spacing, cw) in;
in vec3 tessControlPosition[];

out vec3 tessEvalPosition;
out vec3 tessEvalNormal;
out vec2 tessEvalTextureCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float radius;

vec3 my_norm(vec3 x, float len) {
  return x * len / length(x);
}

void main() {
  vec3 a = gl_TessCoord.x * tessControlPosition[0];
  vec3 b = gl_TessCoord.y * tessControlPosition[1];
  vec3 c = gl_TessCoord.z * tessControlPosition[2];
  vec3 position = my_norm(a + b + c, radius);
  tessEvalNormal = (model * vec4(position / radius, 0.0)).xyz;

  float latitude = acos(position.z / radius) / M_PI;
  float longitude = (atan(position.y, position.x) + M_PI) / (2 * M_PI);
  tessEvalTextureCoord = vec2(longitude, latitude);

  tessEvalPosition = (model * vec4(position, 1)).xyz;
  gl_Position = projection * view * model * vec4(position, 1);
}
