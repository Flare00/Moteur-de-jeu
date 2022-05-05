#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "BoundingBox.hpp"
#include "ModeleCollision.hpp"
#include "Collision.hpp"

class Intersection {


public:
	struct CollisionData {
		bool collide;
		vec3 normal;
		float profondeur;
		std::vector<glm::vec3> contacts;
	};
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

	// --- Intersection Points ---
	static void intersectionAABBtoAABB(BoundingBox* a, BoundingBox* b, CollisionData* data) {
		std::vector<glm::vec3> contacts;
		std::vector<glm::vec3> tmp = a->getCoords();
		float distance = 0.0f;
		for (size_t i = 0, maxTmp = tmp.size(); i < maxTmp; i++)
		{
			float dist = b->inCollision(tmp[i]);
			if (dist >= 0.0f)
			{
				contacts.push_back(tmp[i]);
				distance = glm::max(distance, dist);
			}
		}

		tmp = b->getCoords();
		for (size_t i = 0, maxTmp = tmp.size(); i < maxTmp; i++)
		{
			float dist = a->inCollision(tmp[i]);
			if (dist >= 0.0f)
			{
				contacts.push_back(tmp[i]);
				distance = glm::max(distance, dist);
			}
		}
		data->profondeur = distance;
		data->contacts = contacts;
	}

	static void intersectionModeleToModele(ModeleCollision* a, ModeleCollision* b, CollisionData* data) {
		std::vector<glm::vec3> contacts;

		//todo

		if (contacts.size() > 0) {
			data->collide = false;
		}
		data->contacts = contacts;
	}

	static void intersectionAABBtoModele(BoundingBox* a, ModeleCollision* b, CollisionData* data) {
		std::vector<glm::vec3> contacts;

		//todo
		if (contacts.size() > 0) {
			data->collide = false;
		}
		data->contacts = contacts;
	}

	static CollisionData intersection(Collision* c1, Collision* c2) {
		CollisionData result;
		if (c1->getType() == Collision::BOUNDING_BOX && c2->getType() == Collision::BOUNDING_BOX) {
			BoundingBox* a = (BoundingBox*)c1;
			BoundingBox* b = (BoundingBox*)c2;
			result.collide = isIntersectionAABBtoAABB(a, b);
			if (result.collide) {
				intersectionAABBtoAABB(a, b, &result);
				result.normal = c2->getCenter() - c1->getCenter();
			}
		}
		else if (c1->getType() == Collision::MODELE && c2->getType() == Collision::MODELE) {
			ModeleCollision* a = (ModeleCollision*)c1;
			ModeleCollision* b = (ModeleCollision*)c2;
			result.collide = isIntersectionModeleToModele(a, b);
			if (result.collide) {
				intersectionModeleToModele(a, b, &result);
			}
		}
		else if (c1->getType() == Collision::BOUNDING_BOX && c2->getType() == Collision::MODELE) {
			BoundingBox* a = (BoundingBox*)c1;
			ModeleCollision* b = (ModeleCollision*)c2;
			result.collide = isIntersectionAABBtoModele(a, b);
			if (result.collide) {
				 intersectionAABBtoModele(a, b, &result);
			}
		}
		else if (c1->getType() == Collision::MODELE && c2->getType() == Collision::BOUNDING_BOX) {
			BoundingBox* a = (BoundingBox*)c1;
			ModeleCollision* b = (ModeleCollision*)c2;
			result.collide = isIntersectionAABBtoModele(a, b);
			if (result.collide) {
				intersectionAABBtoModele(a, b, &result);
			}
		}
		std::cout << result.collide << std::endl;
		return result;
	}
};
#endif // !__INTERSECTION_HPP__
