#include "Sphere.hpp"
#include "Texture.hpp"

#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

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
} LightID;

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
} Light;

class CelestialBody {
public:
  double physicsScale;
  
  MaterialID material_id;
  Material material;
  GLuint use_texture_id;
  
  glm::dvec3 position;
  glm::dvec3 velocity;
  double mass;
  Sphere *sphere;

  Texture *texture;

  CelestialBody(GLuint program, double physicsScale, glm::dvec3 position, glm::dvec3 velocity,
		double mass, glm::vec3 color, unsigned int detail,
		float radius, float rotation, float obliquity,
		Material material, Texture *texture);
  ~CelestialBody();
  virtual void render();
  virtual void update(double timeScale);
};

#endif
