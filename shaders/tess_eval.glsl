#version 400 core

layout(triangles, equal_spacing, cw) in;
in vec3 tessControlPosition[];

out vec3 tessEvalPosition;
out vec3 tessEvalNormal;
out vec3 texturePosition;
//out vec2 tessEvalTextureCoord;

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

  //  tessEvalTextureCoord = vec2(longitude, latitude);
  texturePosition = position;
  tessEvalPosition = (model * vec4(position, 1)).xyz;
  gl_Position = projection * view * model * vec4(position, 1);
}
