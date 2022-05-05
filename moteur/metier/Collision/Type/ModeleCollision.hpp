#ifndef __MODELE_COLLISION_HPP__
#define __MODELE_COLLISION_HPP__
#include <vector>
#include <glm/glm.hpp>

#include "Collision.hpp"
using namespace glm;

class ModeleCollision : public Collision{
protected:
	std::vector<vec3> vertices;
	std::vector<unsigned int> indices;

	bool hasData;

public :
	ModeleCollision() : Collision(TypeCollision::MODELE) {
		this->hasData = false;
	}

	ModeleCollision(std::vector<vec3> vertices, std::vector<unsigned int> indices) : Collision(TypeCollision::MODELE) {
		this->vertices = vertices;
		this->indices = indices;
		this->hasData = true;
	}

	void SetData(std::vector<vec3> vertices, std::vector<unsigned int> indices) {
		this->vertices = vertices;
		this->indices = indices;
		this->hasData = true;
	}

	std::vector<vec3> GetVertices() {
		return this->vertices;
	}

	std::vector<unsigned int> GetIndices() {
		return this->indices;
	}

	virtual glm::vec3 getCenter() {
		return glm::vec3(0);
	}
	virtual std::vector<glm::vec3> getCoords() {
		std::vector<vec3> coords(this->vertices);
		for (size_t i, max = coords.size(); i < max; i++) {
			vec4 tmp = this->transformation * glm::vec4(coords[i], 1.0f);
			coords[i].x = tmp.x;
			coords[i].y = tmp.y;
			coords[i].z = tmp.z;
		}

		return vertices;
	}
};

#endif // !__MODELE_COLLISION_HPP__
