#version 400 core

#define MAX_LIGHTS 10
#define M_PI 3.1415926535897932384626433832795

struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  vec3 position;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
  bool nonLambertian;
};


uniform vec3 viewPosition;

uniform uint numLights;
uniform Light lights[MAX_LIGHTS];
uniform Material material;

in vec3 tessEvalPosition;
in vec3 tessEvalNormal;
in vec3 texturePosition;

uniform float radius;
//in vec2 tessEvalTextureCoord;

uniform bool useTexture;
uniform bool useNightTexture;
uniform bool useSpecularTexture;
uniform bool useNormalMap;
uniform sampler2D diffuseDayTexture;
uniform sampler2D diffuseNightTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalMap;

out vec3 color;

// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
   vec3 map = texture(normalMap, texcoord).xyz;
   map = map * 255./127. - 128./127.;
   mat3 TBN = cotangent_frame(N, -V, texcoord);
   return normalize(TBN * map);
}

vec3 calculateLight(Light light) {
  
  float latitude = acos(texturePosition.z / radius) / M_PI;
  float longitude = 1 - (atan(texturePosition.y, texturePosition.x) + M_PI) / (2 * M_PI);

  vec2 tessEvalTextureCoord = vec2(longitude, latitude);
  
  vec3 lightDirection = normalize(light.position - tessEvalPosition);
  vec3 viewDirection = normalize(viewPosition - tessEvalPosition);

  float dist = length(light.position - tessEvalPosition);
  float attenuation = 1.0 / (light.constant + light.linear * dist + 
			      light.quadratic * (dist * dist));

  vec3 normal;

  //TODO
  // if (useNormalMap) {
  //   //normal = texture(normalMap, tessEvalTextureCoord).rgb;
  //   // transform normal vector to range [-1,1]
  //   //normal = normalize(normal * 2.0 - 1.0);
  //   normal = perturb_normal(tessEvalNormal, normalize(viewPosition - tessEvalPosition), tessEvalTextureCoord);
  //   //normal = normalize(texture(normalMap, tessEvalTextureCoord;).rgb * 2.0 - 1.0);
  // } else {
  normal = normalize(tessEvalNormal);
  // }

  if (tessEvalNormal == vec3(0,0,0)) {
     return vec3(1.0,1.0,1.0);
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
	ambientLight = light.ambient * (material.ambient + vec3(0.8));
	diffuseColor *= vec3(texture(diffuseNightTexture,tessEvalTextureCoord));
      }
    } else {
      diffuseColor *= vec3(texture(diffuseDayTexture,tessEvalTextureCoord));
      ambientLight = light.ambient * material.ambient;
    }
  } else {
    ambientLight = light.ambient * material.ambient;
  }

  ambientLight *= attenuation;

  float diffuse = 0;
  // non lambertian reflector (e.g. moon)
  if (material.nonLambertian && dot(normal,lightDirection) > 0) {
    diffuse = 1;
  } else {
    diffuse = max(dot(normal, lightDirection), 0);
  }
  float specular = pow(max(dot(normal, halfwayDirection), 0), material.shininess);

  vec3 diffuseLight = attenuation * light.diffuse * material.diffuse * diffuse;
  vec3 specularLight = attenuation * light.specular * material.specular * specular;

  if (useSpecularTexture) {
    vec3 specularColor = vec3(texture(specularTexture, tessEvalTextureCoord));
    return (ambientLight + diffuseLight) * diffuseColor + specularLight * specularColor * diffuseColor;
  } else {
    return (ambientLight + diffuseLight + specularLight) * diffuseColor;
  }
}

void main() {
  if (radius==0) {
    color = vec3(1.0, 0.0, 0.0);
    return;
  }
  // make everything read if we have too many lights
  if (numLights > MAX_LIGHTS) {
    color = vec3(1.0, 0.0, 0.0);
    return;
  }
  for (uint i = 0; i < numLights; i++) {
    color += calculateLight(lights[i]);
  }
}

