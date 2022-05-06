#ifndef __TERRAIN_COLLISION_HPP__
#define __TERRAIN_COLLISION_HPP__
#include <iostream>
#include <Texture.hpp>
#include <glm/glm.hpp>

#include "Collision.hpp"
#include <GameObject/ModeleComponent.hpp>

class TerrainCollision : public Collision {
private:
	int sizeRow, sizeCol;
	std::vector<float> hauteurs;
	glm::vec3 minCoord, maxCoord;
	float height;
public:
	TerrainCollision(Texture* texture, std::vector<glm::vec3> vertex, float height) : Collision(TypeCollision::TERRAIN) {
		this->sizeCol = texture->getHeight();
		this->sizeRow = texture->getWidth();
		std::vector<glm::vec3> data = texture->getDataVector();
		this->height = height;

		for (size_t i = 0, max = data.size(); i < max; i++) {
			hauteurs.push_back(data[i].x / 255.0f);
		}

		this->minCoord = glm::vec3(FLT_MAX);
		this->maxCoord = glm::vec3(-FLT_MAX);
		for (size_t i = 0, max = vertex.size(); i < max; i++) {
			for (int j = 0; j < 3; j++) {
				if (this->minCoord[j] < vertex[i][j]) {
					this->minCoord[j] = vertex[i][j];
				}
				if (this->maxCoord[j] > vertex[i][j]) {
					this->maxCoord[j] = vertex[i][j];
				}
			}
		}


	}

	glm::vec3 getApplyPoint(glm::vec3 a) {
		glm::vec4 tmp = this->transformation * glm::vec4(a, 1);
		return glm::vec3(tmp.x, tmp.y, tmp.z);
	}

	float distanceZTerrain(glm::vec3 coord) {
		float res = -FLT_MAX;

		glm::vec3 minTransform = getApplyPoint(this->minCoord);
		glm::vec3 maxTransform = getApplyPoint(this->maxCoord);

		glm::vec3 p = coord - glm::vec3(minTransform.x, minTransform.y, minTransform.z);
		glm::vec3 maxmin = maxTransform - minTransform;
		if (p.x >= 0 && p.z >= 0 && p.x <= maxmin.x && p.z <= maxmin.z)
		{
			int curX = (int)(p.x * (this->sizeRow / maxmin.x));
			int curZ = (int)(p.z * (this->sizeCol / maxmin.z));
			res = this->hauteurs[(curZ * this->sizeRow) + curX] + this->height;
		}
		return res;
	}
};

#endif // !__TERRAIN_COLLISION_HPP__
