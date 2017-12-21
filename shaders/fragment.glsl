#version 330 core

struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
  bool nonLambertian;
};


uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform Light light;
uniform Material material;

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec3 fragmentColor;
in vec2 fragmentTexture;

uniform bool useTexture;
uniform bool useNightTexture;
uniform sampler2D diffuseDayTexture;
uniform sampler2D diffuseNightTexture;

out vec3 color;

void main() {
  vec3 lightDirection = normalize(lightPosition - fragmentPosition);
  vec3 viewDirection = normalize(viewPosition - fragmentPosition);
  vec3 normal = normalize(fragmentNormal);
  vec3 halfwayDirection = normalize(viewDirection + lightDirection);
  
  vec3 ambientLight = light.ambient * material.ambient;
  float diffuse = 0;
  // non lambertian reflector (e.g. moon)
  if (material.nonLambertian && dot(normal,lightDirection) > 0) {
    diffuse = 1;
  } else {
    diffuse = max(dot(normal, lightDirection), 0);
  }
  vec3 diffuseLight = light.diffuse * material.diffuse * diffuse;
  float specular = pow(max(dot(normal, halfwayDirection), 0), material.shininess);
  vec3 specularLight = light.specular * material.specular * specular;

  vec3 baseColor = fragmentColor;
  if (useTexture) {
    if (useNightTexture) {
      if (dot(normal,lightDirection) > 0) {
	baseColor *= vec3(texture(diffuseDayTexture,fragmentTexture));
      } else {
	baseColor *= vec3(texture(diffuseNightTexture,fragmentTexture));
      }
    } else {
    }
    baseColor *= vec3(texture(diffuseDayTexture,fragmentTexture));
  }
  
  color = (ambientLight + diffuseLight + specularLight) * baseColor;
}

