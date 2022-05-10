#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <glm/glm.hpp>

using namespace glm;
class Material {
private :
	vec3 ambiant;
	vec3 diffuse;
	vec3 specular;
	float shininess;
public :
	Material(vec3 diffuse = vec3(1.0f), vec3 specular = vec3(0.2f), vec3 ambiant = vec3(0.00f), float shininess = 0.2f) {
		this->ambiant = ambiant;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}

	void setAmbiant(vec3 ambiant) {
		this->ambiant = ambiant;
	}
	void setDiffuse(vec3 diffuse) {
		this->diffuse = diffuse;
	}
	void setSpecular(vec3 specular) {
		this->specular = specular;
	}
	void setShininess(float shininess) {
		this->shininess = shininess;
	}

	vec3 getAmbiant() {
		return this->ambiant;
	}
	vec3 getDiffuse() {
		return this->diffuse;
	}
	vec3 getSpecular() {
		return this->specular;
	}
	float getShininess() {
		return this->shininess;
	}
};

#endif // !__MATERIAL_H__
