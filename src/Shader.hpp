#include "headers.h"

#ifndef SHADER_H
#define SHADER_H

#define MAIN_VERTEX_SOURCE "./shaders/vertex.glsl"
#define MAIN_TESS_CONTROL_SOURCE "./shaders/tess_control.glsl"
#define MAIN_TESS_EVAL_SOURCE "./shaders/tess_eval.glsl"
#define MAIN_FRAGMENT_SOURCE "./shaders/fragment.glsl"
#define ORBIT_VERTEX_SOURCE "./shaders/orbit_vertex.glsl"
#define ORBIT_FRAGMENT_SOURCE "./shaders/orbit_fragment.glsl"
#define TEXT_VERTEX_SOURCE "./shaders/text_vertex.glsl"
#define TEXT_FRAGMENT_SOURCE "./shaders/text_fragment.glsl"

typedef struct _Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
  bool nonLambertian;
} Material;

typedef struct _Light {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;

  glm::vec3 position;
} Light;


typedef struct _MaterialID {
  GLint ambient;
  GLint diffuse;
  GLint specular;
  GLint shininess;
  GLint nonLambertian;
} MaterialID;

typedef struct _LightID {
  GLint ambient;
  GLint diffuse;
  GLint specular;
  GLint constant;
  GLint linear;
  GLint quadratic;
  GLint position;
} LightID;


class Shader {
protected:
  GLuint programID;
public:
  Shader(GLuint programID);
  virtual void use();
};

class MainShader : public Shader {
private:
  GLuint projectionID;
  GLuint viewID;
  GLuint modelID;

  MaterialID materialID;
  LightID lightID;
  GLuint numLightsID;

  GLuint radiusID;
  GLuint detailID;

  GLuint useTextureID;
  GLuint useNightID;
  GLuint useSpecularID;
  GLuint useNormalID;

  GLuint viewPositionID;
  
public:
  MainShader();
  ~MainShader();
  void setProjection(glm::mat4 projection);
  void setView(glm::mat4 view);
  void setModel(glm::mat4 model);
  void setMaterial(Material material);
  void setLight(Light light, size_t i);
  void setNumLights(unsigned int numLights);
  void setRadius(float radius);
  void setDetail(unsigned int detail);
  void setUseTexture(bool useTexture);
  void setTextureIDs(bool useNight, bool useSpecular, bool useNormal);
  void setTextureSettings(bool useNight, bool useSpecular, bool useNormal);
  void setViewPosition(glm::vec3 position);
};

class OrbitShader : public Shader {
private:
  GLuint projectionID;
  GLuint viewID;
  GLuint modelID;

public:
  OrbitShader();
  ~OrbitShader();
  void setProjection(glm::mat4 projection);
  void setView(glm::mat4 view);
  void setModel(glm::mat4 model);
};

class TextShader : public Shader {
private:
  GLuint projectionID;
  GLuint textColorID;
  //  GLuint viewID;
  // GLuint modelID;

public:
  TextShader();
  ~TextShader();
  void setProjection(glm::mat4 projection);
  void setTextColor(glm::vec3 color);
  void setTextureIDs();
  //void setView(glm::mat4 view);
  //void setModel(glm::mat4 model);
};
#endif
