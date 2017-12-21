#version 330 core

struct LightingParameters {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform LightingParameters lightParameters;
uniform LightingParameters materialParameters;
uniform float shininess;

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec3 fragmentColor;

out vec3 color;

void main() {
  vec3 lightDirection = normalize(lightPosition - fragmentPosition);
  vec3 viewDirection = normalize(viewPosition - fragmentPosition);
  vec3 normal = normalize(fragmentNormal);
  vec3 halfwayDirection = normalize(viewDirection + lightDirection);
  
  vec3 ambientLight = lightParameters.ambient * materialParameters.ambient;
  vec3 diffuseLight = lightParameters.diffuse * materialParameters.diffuse * max(dot(normal, lightDirection), 0);
  vec3 specularLight = lightParameters.specular * materialParameters.specular * pow(max(dot(normal, halfwayDirection), 0), shininess);

  color = (ambientLight + diffuseLight + specularLight) * fragmentColor;
 
  //color = ambientLight * fragmentColor;
}

