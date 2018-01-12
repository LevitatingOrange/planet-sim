#version 400 core

layout(vertices = 3) out;
in vec3 vertexPosition[];
out vec3 tessControlPosition[];

uniform uint detail;

void main() {
  tessControlPosition[gl_InvocationID] = vertexPosition[gl_InvocationID];
  if (gl_InvocationID == 0) {
    gl_TessLevelInner[0] = detail;
    gl_TessLevelOuter[0] = detail;
    gl_TessLevelOuter[1] = detail;
    gl_TessLevelOuter[2] = detail;
  }
}
