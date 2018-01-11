#version 400 core

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

in vec3 tessEvalPosition;
in vec3 tessEvalNormal;
in vec2 tessEvalTextureCoord;

uniform bool useTexture;
uniform bool useNightTexture;
uniform bool useSpecularTexture;
uniform bool useNormalMap;
uniform sampler2D diffuseDayTexture;
uniform sampler2D diffuseNightTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalMap;

out vec3 color;

void main() {
  vec3 lightDirection = normalize(lightPosition - tessEvalPosition);
  vec3 viewDirection = normalize(viewPosition - tessEvalPosition);

  vec3 normal;

  // TODO
  //if (useNormalMap) {
  //   normal = normalize(texture(normalMap, tessEvalTextureCoord;).rgb * 2.0 - 1.0);
  // } else {
  normal = normalize(tessEvalNormal);
  //}

  if (tessEvalNormal == vec3(0,0,0)) {
    color = vec3(1.0,1.0,1.0);
    return;
  }
  
  vec3 halfwayDirection = normalize(viewDirection + lightDirection);
  
  vec3 diffuseColor = vec3(1.0,1.0,1.0);
  vec3 ambientLight;
  if (useTexture) {
    if (useNightTexture) {
      if (dot(normal,lightDirection) > 0) {
	diffuseColor *= vec3(texture(diffuseDayTexture,tessEvalTextureCoord));
      } else {
	// because night maps are already dark
	ambientLight = light.ambient * (material.ambient + vec3(0.2));
	diffuseColor *= vec3(texture(diffuseNightTexture,tessEvalTextureCoord));
      }
    } else {
      diffuseColor *= vec3(texture(diffuseDayTexture,tessEvalTextureCoord));
      ambientLight = light.ambient * material.ambient;
    }
  } else {
    ambientLight = light.ambient * material.ambient;
  }

  float diffuse = 0;
  // non lambertian reflector (e.g. moon)
  if (material.nonLambertian && dot(normal,lightDirection) > 0) {
    diffuse = 1;
  } else {
    diffuse = max(dot(normal, lightDirection), 0);
  }
  float specular = pow(max(dot(normal, halfwayDirection), 0), material.shininess);

  vec3 diffuseLight = light.diffuse * material.diffuse * diffuse;
  vec3 specularLight = light.specular * material.specular * specular;

  if (useSpecularTexture) {
    vec3 specularColor = vec3(texture(specularTexture, tessEvalTextureCoord));
    color = (ambientLight + diffuseLight) * diffuseColor + specularLight * specularColor * diffuseColor;
  } else {
    color = (ambientLight + diffuseLight + specularLight) * diffuseColor;
  }
}

