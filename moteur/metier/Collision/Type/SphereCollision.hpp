#ifndef __SPHERE_COLLISION_HPP__
#define __SPHERE_COLLISION_HPP__

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cmath>

#include "Collision.hpp"

using namespace glm;

class SphereCollision : public Collision
{
protected:
	glm::vec3 center;
	float radius;
	bool hasData;
public:
	SphereCollision() : Collision(TypeCollision::SPHERE) {
		this->hasData = false;
	}

	SphereCollision(vec3 center, float radius) : Collision(TypeCollision::SPHERE) {
		this->center = center;
		this->radius = radius;
		this->hasData = true;
	}

	void SetData(vec3 center, float radius) {
		this->center = center;
		this->radius = radius;
		this->hasData = true;
	}

	vec3 getCenter() {
		return this->center;
	}
	float getRadius() {
		return this->radius;
	}
};

#endif // !__SPHERE_COLLISION_HPP__
