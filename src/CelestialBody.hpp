#include "Sphere.hpp"

#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

typedef struct _ParameterID {
  GLint ambient;
  GLint diffuse;
  GLint specular;
} ParameterID;

typedef struct _Parameter {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
} Parameter;

class CelestialBody {
public:
  ParameterID material_id;
  Parameter material;
  GLint shininess_id;
  float shininess;
  
  glm::vec3 position;
  glm::vec3 velocity;
  float mass;
  Sphere *sphere;

  CelestialBody(GLuint program, glm::vec3 position, glm::vec3 velocity,
		float mass, glm::vec3 color, unsigned int detail,
		float radius, float rotation, float obliquity,
		Parameter material, float shininess);
  ~CelestialBody();
  virtual void render();
  virtual void update();
};

#endif
