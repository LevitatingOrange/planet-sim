#include "config.hpp"
#include "util.hpp"
#include "CelestialBody.hpp"
#include "Star.hpp"
#include <vector>
#include <iostream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace rapidjson;

void my_assert(bool cond, std::string msg) {
  if (!cond) {
    throw msg;
  }
}

unsigned int get_uint(Value& b, const char* name, int def) {
  if (b.HasMember(name) && b[name].IsInt()) {
    int result = b[name].GetInt();
    return result >= 0? result : def;
  } else {
    return def;
  }
}

double get_double(Value& b, const char* name, double def) {
  if (b.HasMember(name) && (b[name].IsDouble()  || b[name].IsInt())) {
    double result = b[name].GetDouble();
    return result;
  } else {
    return def;
  }
}

double get_doubleb(Value& b, const char* name, double def) {
  if (b.HasMember(name) && (b[name].IsDouble() || b[name].IsInt())) {
    double result = b[name].GetDouble();
    return result >= 0? result : def;
  } else {
    return def;
  }
}

glm::vec3 get_vec3(Value& b, const char* name, float def) {
  if (b.HasMember(name) && b[name].IsArray()) {
    auto arr = b[name].GetArray();
    if (arr.Size() != 3) {
      return glm::vec3(def);
    }
    my_assert(arr[0].IsDouble(), "expected double in vector (pos 0) in " + std::string(name));
    my_assert(arr[1].IsDouble(), "expected double in vector (pos 1) in " + std::string(name));
    my_assert(arr[2].IsDouble(), "expected double in vector (pos 2) in " + std::string(name));
    return glm::dvec3(arr[0].GetDouble(), arr[1].GetDouble(),  arr[2].GetDouble());
  } else {
    return glm::dvec3(def);
  }
}
glm::dvec3 get_dvec3(Value& b, const char* name, double def, double scale) {
  if (b.HasMember(name) && b[name].IsArray()) {
    auto arr = b[name].GetArray();
    if (arr.Size() != 3) {
      return glm::vec3(def);
    }
    my_assert(arr[0].IsDouble(), "expected double in vector (pos 0) in " + std::string(name));
    my_assert(arr[1].IsDouble(), "expected double in vector (pos 1) in " + std::string(name));
    my_assert(arr[2].IsDouble(), "expected double in vector (pos 2) in " + std::string(name));
    return glm::dvec3(arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble());
  } else {
    return glm::dvec3(def);
  }
}

const char* get_str(Value& b, const char* name, char* def) {
  if (b.HasMember(name) && b[name].IsString()) {
    return b[name].GetString();
  } else {
    return def;
  }
}

Universe* readConfig(const char* configPath, GLuint program, float updateTime, GLuint width, GLuint height) {
  Document document;
  document.Parse(readFile(configPath).c_str());
  my_assert(document.IsObject(), "Config file malformed, root has to be object");
  my_assert(document.HasMember("bodies") && document["bodies"].IsArray(), "Config file missing array");

  double physicsScale = get_doubleb(document, "physicsScale", 1.0);
  double radiiScale = get_doubleb(document, "radiiScale", physicsScale);
  double timeScale = get_doubleb(document, "timeScale", physicsScale);
  double g = get_doubleb(document, "g", 1.0);

  Universe* u = new Universe(g, program, timeScale, updateTime, width, height);

  for (auto& b : document["bodies"].GetArray()) {
    my_assert(b.IsObject(), "All bodies have to be objects");
    
    glm::dvec3 position = get_dvec3(b, "position", 0.0, physicsScale);
    glm::dvec3 velocity = get_dvec3(b, "velocity", 0.0, physicsScale);
    double mass = get_doubleb(b, "mass", 0.0);
    glm::vec3 color = glm::vec3(1.0);
    float radius = (float) (radiiScale * get_doubleb(b, "radius", 1.0));
    float rotation = get_double(b, "rotation", 0.0);
    float obliquity = get_double(b, "obliquity", 0.0);
    Material material = {glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0), 1, false};
    Light light = {glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0)};
    const char* diffusePath = get_str(b, "diffusePath", nullptr);
    const char* diffuseNightPath = get_str(b, "diffuseNightPath", nullptr);
    const char* specularPath = get_str(b, "specularPath", nullptr);
    const char* normalPath = get_str(b, "normalPath", nullptr);


    if (b.HasMember("material") && b["material"].IsObject()) {
      Value mat = b["material"].GetObject();
      material.ambient = get_vec3(mat, "ambient", 1.0);
      material.diffuse = get_vec3(mat, "diffuse", 1.0);
      material.specular = get_vec3(mat, "specular", 1.0);
      material.shininess = get_uint(mat, "shininess", 0);
      if (b.HasMember("nonLambertian") && b["nonLambertian"].IsBool() && b["nonLambertian"].GetBool()) {
	material.nonLambertian = true;
      }
    }
    if (b.HasMember("light") && b["light"].IsObject()) {
      Value mat = b["light"].GetObject();
      light.ambient = get_vec3(mat, "ambient", 1.0);
      light.diffuse = get_vec3(mat, "diffuse", 1.0);
      light.specular = get_vec3(mat, "specular", 1.0);
    }

    if (b.HasMember("isStar") && b["isStar"].IsBool() && b["isStar"].GetBool()) {
      u->bodies.push_back(new Star(program, physicsScale, position, velocity, mass, color, radius, rotation, obliquity, material, light,
				   diffusePath != nullptr? new Texture(program, diffusePath, diffuseNightPath, specularPath, normalPath) : nullptr));
    } else {
      u->bodies.push_back(new CelestialBody(program, physicsScale, position, velocity, mass, color, radius, rotation, obliquity, material,
					    diffusePath != nullptr? new Texture(program, diffusePath, diffuseNightPath, specularPath, normalPath) : nullptr));
    }
  }
  return u;
}
