#ifndef __DIRECTIONNAL_LIGHT_HPP__
#define __DIRECTIONNAL_LIGHT_HPP__
#include <glm/glm.hpp>
#include <Light/PointLight.hpp>

using namespace glm;

class DirectionnalLight : public PointLight {
protected:
	glm::vec3 direction;
public :
	DirectionnalLight(vec3 direction, vec3 position, vec3 color = vec3(1), float intensity = 1.0f) : PointLight(position, color, intensity) {
		this->direction = normalize(direction);
	}

	virtual void compute() {
		//this->shadow.
	}

	vec3 getDirection() {
		return this->position;
	}

	void setDirection(vec3 direction) {
		this->direction = normalize(direction);
	}


};

#endif
