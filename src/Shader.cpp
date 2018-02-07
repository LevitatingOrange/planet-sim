#include "Shader.hpp"

#include <iostream>

Shader::Shader(GLuint programID): programID(programID) {};

void Shader::use() {
  glUseProgram(programID);
}

MainShader::MainShader(): Shader(createProgram(MAIN_VERTEX_SOURCE,
					       MAIN_TESS_CONTROL_SOURCE,
					       MAIN_TESS_EVAL_SOURCE,
					       MAIN_FRAGMENT_SOURCE)),
			  projectionID(glGetUniformLocation(programID, "projection")),
			  viewID(glGetUniformLocation(programID, "view")),
			  modelID(glGetUniformLocation(programID, "model")),
			  materialID(MaterialID {
			      glGetUniformLocation(programID, "material.ambient"),
				glGetUniformLocation(programID, "material.diffuse"),
				glGetUniformLocation(programID, "material.specular"),
				glGetUniformLocation(programID, "material.shininess"),
				glGetUniformLocation(programID, "material.nonLambertian")}),
			  lightID(LightID {
			      glGetUniformLocation(programID, "lights.ambient"),
				glGetUniformLocation(programID, "lights.diffuse"),
				glGetUniformLocation(programID, "lights.specular"),
				glGetUniformLocation(programID, "lights.constant"),
				glGetUniformLocation(programID, "lights.linear"),
				glGetUniformLocation(programID, "lights.quadratic"),
				glGetUniformLocation(programID, "lights.position")}),
			  numLightsID(glGetUniformLocation(programID, "numLights")),
			  radiusID(glGetUniformLocation(programID, "radius")),
			  detailID(glGetUniformLocation(programID, "detail")),
			  useTextureID(glGetUniformLocation(programID, "useTexture")),
			  useNightID(glGetUniformLocation(programID, "useNightTexture")),
			  useSpecularID(glGetUniformLocation(programID, "useSpecularTexture")),
			  useNormalID(glGetUniformLocation(programID, "useNormalMap")),
			  viewPositionID(glGetUniformLocation(programID, "viewPosition"))
{
} 

MainShader::~MainShader() {
}

void MainShader::setProjection(glm::mat4 projection) {
  glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
}

void MainShader::setView(glm::mat4 view) {
  glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
}

void MainShader::setModel(glm::mat4 model) {
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
}


void MainShader::setMaterial(Material material) {
  glUniform3fv(materialID.ambient, 1, (GLfloat*) &material.ambient);
  glUniform3fv(materialID.diffuse, 1, (GLfloat*) &material.diffuse);
  glUniform3fv(materialID.specular, 1, (GLfloat*) &material.specular);
  glUniform1f(materialID.shininess, (GLfloat) material.shininess);
  glUniform1i(materialID.nonLambertian, (GLint) material.nonLambertian);

}

void MainShader::setLight(Light light, size_t i) {
  std::string pre = "lights[" + std::to_string(i);
  std::string amb = pre + std::string("].ambient");
  std::string dif = pre + std::string("].diffuse");
  std::string spe = pre + std::string("].specular");
  std::string con = pre + std::string("].constant");
  std::string lin = pre + std::string("].linear");
  std::string qud = pre + std::string("].quadratic");
  std::string pos = pre + std::string("].position");
  lightID = LightID {
    glGetUniformLocation(programID, (char*) amb.c_str()),
    glGetUniformLocation(programID, (char*) dif.c_str()),
    glGetUniformLocation(programID, (char*) spe.c_str()),
    glGetUniformLocation(programID, (char*) con.c_str()),
    glGetUniformLocation(programID, (char*) lin.c_str()),
    glGetUniformLocation(programID, (char*) qud.c_str()),
    glGetUniformLocation(programID, (char*) pos.c_str())};
  
  glUniform3fv(lightID.ambient, 1, (GLfloat*) &light.ambient);
  glUniform3fv(lightID.diffuse, 1, (GLfloat*) &light.diffuse);
  glUniform3fv(lightID.specular, 1, (GLfloat*) &light.specular);
  glUniform1f(lightID.constant, (GLfloat) light.constant);
  glUniform1f(lightID.linear, (GLfloat) light.linear);
  glUniform1f(lightID.quadratic, (GLfloat) light.quadratic);
  glUniform3fv(lightID.position, 1, (GLfloat*) &light.position);
}

void MainShader::setNumLights(unsigned int numLights) {
  glUniform1ui(numLightsID, numLights);
}

void MainShader::setRadius(float radius) {
  glUniform1f(radiusID, radius);
}
void MainShader::setDetail(unsigned int detail) {
  glUniform1ui(detailID, detail);
}

void MainShader::setUseTexture(bool useTexture) {
  glUniform1i(useTextureID, useTexture);
}

void MainShader::setTextureIDs(bool useNight, bool useSpecular, bool useNormal) {
  glUniform1i(glGetUniformLocation(programID, "diffuseDayTexture"), 0);
  if (useNight) {
    glUniform1i(glGetUniformLocation(programID, "diffuseNightTexture"), 1);
  }
  if (useSpecular) {
    glUniform1i(glGetUniformLocation(programID, "specularTexture"), 2);
  }
  if (useNormal) {
    glUniform1i(glGetUniformLocation(programID, "normalMap"), 3);
  }
}
void MainShader::setTextureSettings(bool useNight, bool useSpecular, bool useNormal) {
  glUniform1i(useNightID, useNight);
  glUniform1i(useSpecularID, useSpecular);
  glUniform1i(useNormalID, useNormal);
}
void MainShader::setViewPosition(glm::vec3 position) {
    glUniform3fv(viewPositionID, 1, (GLfloat*) &position);
}
			   

OrbitShader::OrbitShader(): Shader(createProgram(ORBIT_VERTEX_SOURCE,
						 nullptr,
						 nullptr,
					       ORBIT_FRAGMENT_SOURCE)),
			  projectionID(glGetUniformLocation(programID, "projection")),
			  viewID(glGetUniformLocation(programID, "view")),
			  modelID(glGetUniformLocation(programID, "model"))
{
} 

OrbitShader::~OrbitShader() {
}


void OrbitShader::setProjection(glm::mat4 projection) {
  glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
}

void OrbitShader::setView(glm::mat4 view) {
  glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
}

void OrbitShader::setModel(glm::mat4 model) {
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
}

TextShader::TextShader(): Shader(createProgram(TEXT_VERTEX_SOURCE,
						 nullptr,
						 nullptr,
					       TEXT_FRAGMENT_SOURCE)),
			  projectionID(glGetUniformLocation(programID, "projection")),
			  textColorID(glGetUniformLocation(programID, "textColor"))
			  //			  viewID(glGetUniformLocation(programID, "view")),
			  //			  modelID(glGetUniformLocation(programID, "model"))
{
} 

TextShader::~TextShader() {
}


void TextShader::setProjection(glm::mat4 projection) {
  glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
}

void TextShader::setTextColor(glm::vec3 color) {
  glUniform3f(textColorID, color.x, color.y, color.z);
}

// void TextShader::setView(glm::mat4 view) {
//   glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
// }

// void TextShader::setModel(glm::mat4 model) {
//   glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
// }



