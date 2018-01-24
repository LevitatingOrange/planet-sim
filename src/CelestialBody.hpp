#include "headers.h"
#include <deque>
#include <vector>

#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

class CelestialBody {
public:
  std::string name;
  
  bool glInited;
  
  bool isStar;
  MainShader* mainShader;
  
  double physicsScale;
  
  Material material;

  std::deque<glm::vec3> positions;
  size_t orbitSize;
  bool orbitCompleted;
  GLuint vertexArray = 0;
  GLuint vertexBuffer = 0;
  GLuint indexBuffer = 0;
  std::vector<unsigned int> indices;
  
  glm::dvec3 position;
  glm::dvec3 velocity;
  double mass;
  Sphere *sphere;

  Texture *texture;

  CelestialBody(std::string name, MainShader* mainShader, double physicsScale, size_t orbitSize, glm::dvec3 position, glm::dvec3 velocity,
		double mass, glm::vec3 color,
		float radius, float rotation, float obliquity,
		Material material, Texture *texture);
  ~CelestialBody();
  virtual void render(glm::vec3 viewPosition);
  virtual void renderOrbit();
  virtual void update(double timeScale);
  void initGL();
};

#endif
