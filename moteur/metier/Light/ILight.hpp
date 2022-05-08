#ifndef __I_LIGHT_HPP__
#define __I_LIGHT_HPP__

#include <glm/glm.hpp>
#include <Light/ShadowMap.hpp>
using namespace glm;

class ILight {
protected:
	ShadowMap shadow;
	vec3 color;
	float intensity;
public :

	/// <summary>
	/// shadowResolution de 1 à 12, (calcul 2^n, si shadowResolution = 4 alors shadow map de taille 16, si shadowResolution = 12 alors shadow map de taille 4096)
	/// </summary>
	ILight(vec3 color = vec3(1.0f), float intensity = 1.0f, int shadowResolution = 10) : shadow(shadowResolution) {
		this->color = color;
		this->intensity = intensity;
	}

	virtual void compute() = 0;

	vec3 getColor() {
		return this->color;
	}
	float getIntensity() {
		return this->intensity;
	}

	void setColor(vec3 color) {
		this->color = color;
	}
	void setIntensity(float intensity) {
		this->intensity = intensity;
	}

	ShadowMap* getShadowMap() {
		return &this->shadow;
	}

};

#endif // !__I_LIGHT_HPP__
