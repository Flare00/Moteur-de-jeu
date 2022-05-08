#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>
#include <Light/ILight.hpp>

using namespace glm;

class PointLight : public ILight {
protected:
	vec3 position;

public:
	PointLight(vec3 position = vec3(0), vec3 color = vec3(1), float intensity = 1.0f) : ILight(color, intensity) {
		this->position = position;
	}

	virtual void compute() {

	}

	vec3 getPosition() {
		return this->position;
	}

	void setPosition(vec3 position) {
		this->position = position;
	}
};

#endif