#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "BoundingBox.hpp"
#include "ModeleCollision.hpp"
#include "Collision.hpp"

class Intersection {
public:
	// --- Is Intersection ---
	static bool isIntersectionAABBtoAABB(BoundingBox* a, BoundingBox* b) {
		glm::vec3 aMin = a->getMin();
		glm::vec3 aMax = a->getMax();
		glm::vec3 bMin = b->getMin();
		glm::vec3 bMax = b->getMax();

		return aMin.x <= bMax.x && aMax.x >= bMin.x &&
			aMin.y <= bMax.y && aMax.y >= bMin.y &&
			aMin.z <= bMax.z && aMax.z >= bMin.z;
	}

	static bool isIntersectionModeleToModele(ModeleCollision* a, ModeleCollision* b) {

		return false;
	}

	static bool isIntersectionAABBtoModele(BoundingBox* a, ModeleCollision* b) {
		return false;
	}

	static bool isIntersection(Collision* c1, Collision* c2) {
		bool result = false;
		if (c1->getType() == Collision::BOUNDING_BOX && c2->getType() == Collision::BOUNDING_BOX) {
			result = isIntersectionAABBtoAABB((BoundingBox*)c1, (BoundingBox*)c2);
		}
		else if (c1->getType() == Collision::MODELE && c2->getType() == Collision::MODELE) {
			result = isIntersectionModeleToModele((ModeleCollision*)c1, (ModeleCollision*)c2);
		}
		else if (c1->getType() == Collision::BOUNDING_BOX && c2->getType() == Collision::MODELE) {
			result = isIntersectionAABBtoModele((BoundingBox*)c1, (ModeleCollision*)c2);
		}
		else if (c1->getType() == Collision::MODELE && c2->getType() == Collision::BOUNDING_BOX) {
			result = isIntersectionAABBtoModele((BoundingBox*)c2, (ModeleCollision*)c1);
		}
		return result;
	}

	// --- Intersection Points ---
	static std::vector<glm::vec3> intersectionPointsAABBtoAABB(BoundingBox* a, BoundingBox* b) {
		std::vector<glm::vec3> res;
		std::vector<glm::vec3> tmp = a->getCoords();

		for (size_t i = 0, maxTmp = tmp.size(); i < maxTmp; i++)
		{
			if (b->isInCollision(tmp[i]))
			{
				res.push_back(tmp[i]);
			}
		}

		tmp = b->getCoords();
		for (size_t i = 0, maxTmp = tmp.size(); i < maxTmp; i++)
		{
			if (a->isInCollision(tmp[i]))
			{
				res.push_back(tmp[i]);
			}
		}

		return res;
	}

	static std::vector<glm::vec3> intersectionPointsModeleToModele(ModeleCollision* a, ModeleCollision* b) {
		return std::vector<glm::vec3>();
	}

	static std::vector<glm::vec3> intersectionPointsAABBtoModele(BoundingBox* a, ModeleCollision* b) {
		return std::vector<glm::vec3>();
	}

	static std::vector<glm::vec3> intersectionPoints(Collision* c1, Collision* c2) {
		std::vector<glm::vec3> result = std::vector<glm::vec3>();
		if (c1->getType() == Collision::BOUNDING_BOX && c2->getType() == Collision::BOUNDING_BOX) {
			result = intersectionPointsAABBtoAABB((BoundingBox*)c1, (BoundingBox*)c2);
		}
		else if (c1->getType() == Collision::MODELE && c2->getType() == Collision::MODELE) {
			result = intersectionPointsModeleToModele((ModeleCollision*)c1, (ModeleCollision*)c2);
		}
		else if (c1->getType() == Collision::BOUNDING_BOX && c2->getType() == Collision::MODELE) {
			result = intersectionPointsAABBtoModele((BoundingBox*)c1, (ModeleCollision*)c2);
		}
		else if (c1->getType() == Collision::MODELE && c2->getType() == Collision::BOUNDING_BOX) {
			result = intersectionPointsAABBtoModele((BoundingBox*)c2, (ModeleCollision*)c1);
		}
		return result;
	}
};
#endif // !__INTERSECTION_HPP__
