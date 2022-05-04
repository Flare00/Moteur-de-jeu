#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>
using namespace glm;

class Light {
private:
	vec3 position;
	vec3 color;
	float intensity;
public:
	Light(vec3 position = vec3(0), vec3 color = vec3(1), float intensity = 1.0f) {
		this->position = position;
		this->color = color;
		this->intensity = intensity;
	}

	vec3 getPosition() {
		return this->position;
	}
	vec3 getColor() {
		return this->color;
	}
	float getIntensity() {
		return this->intensity;
	}

	void setPosition(vec3 position) {
		this->position = position;
	}
	void setColor(vec3 color) {
		this->color = color;
	}
	void setIntensity(float intensity) {
		this->intensity = intensity;
	}
};

#endif