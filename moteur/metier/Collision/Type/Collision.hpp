#ifndef __COLLISION_HPP__
#define __COLLISION_HPP__

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cmath>

class Collision {
public :
	enum TypeCollision {
		BOUNDING_BOX,
		SPHERE,
		MODELE
	};
protected :
	glm::mat4 transformation;
	TypeCollision type;
public :
	Collision(TypeCollision type) {
		this->type = type;
	}
	TypeCollision getType() {
		return this->type;
	}

	virtual glm::vec3 getCenter() {
		return glm::vec3(0);
	}
	virtual std::vector<glm::vec3> getCoords() {
		return std::vector<glm::vec3>();
	}
	virtual void applyTransformation(glm::mat4 t){
		transformation = t;
	}
};

#endif