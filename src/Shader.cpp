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
			      glGetUniformLocation(programID, "light1.ambient"),
				glGetUniformLocation(programID, "light1.diffuse"),
				glGetUniformLocation(programID, "light1.specular"),
				glGetUniformLocation(programID, "light1.constant"),
				glGetUniformLocation(programID, "light1.linear"),
				glGetUniformLocation(programID, "light1.quadratic"),
				glGetUniformLocation(programID, "light1.position")}),
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

void MainShader::setLight(Light light) {
  glUniform3fv(lightID.ambient, 1, (GLfloat*) &light.ambient);
  glUniform3fv(lightID.diffuse, 1, (GLfloat*) &light.diffuse);
  glUniform3fv(lightID.specular, 1, (GLfloat*) &light.specular);
  glUniform1f(lightID.constant, (GLfloat) light.constant);
  glUniform1f(lightID.linear, (GLfloat) light.linear);
  glUniform1f(lightID.quadratic, (GLfloat) light.quadratic);
  glUniform3fv(lightID.position, 1, (GLfloat*) &light.position);
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

