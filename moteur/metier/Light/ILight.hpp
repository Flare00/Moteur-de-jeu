#ifndef __I_LIGHT_HPP__
#define __I_LIGHT_HPP__

#include <glm/glm.hpp>
#include <Light/ShadowMap.hpp>
#include <GameObject/GameObject.hpp>
using namespace glm;

class ILight {
public:
	enum Type {
		DIRECTIONAL
	};
protected:
	vec3 position;
	vec3 color;
	float intensity;
	Type type;
	bool dirty = false;
public :


	ILight(Type t, vec3 position = vec3(0), vec3 color = vec3(1.0f), float intensity = 1.0f)  {
		this->color = color;
		this->intensity = intensity;
		this->type = t;
		this->position = position;
	}

	virtual void compute(GameObject* scene) = 0;

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

	Type getType() {
		return this->type;
	}

	vec3 getPosition() {
		return this->position;
	}

	void setPosition(vec3 position) {
		this->position = position;
		this->dirty = true;
	}
};

#endif // !__I_LIGHT_HPP__
