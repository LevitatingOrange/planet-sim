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
uniform bool nonLambertian = false;

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
  float diffuse = 0;
  // non lambertian reflector (e.g. moon)
  if (nonLambertian && dot(normal,lightDirection) > 0) {
    diffuse = 1;
  } else {
    diffuse = max(dot(normal, lightDirection), 0);
  }
  vec3 diffuseLight = lightParameters.diffuse * materialParameters.diffuse * diffuse;
  float specular = pow(max(dot(normal, halfwayDirection), 0), shininess);
  vec3 specularLight = lightParameters.specular * materialParameters.specular * specular;

  color = (ambientLight + diffuseLight + specularLight) * fragmentColor;
 
  //color = ambientLight * fragmentColor;
}

