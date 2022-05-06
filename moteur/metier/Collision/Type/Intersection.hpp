#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "BoundingBox.hpp"
#include "ModeleCollision.hpp"
#include "Collision.hpp"
#include <glm/gtx/normal.hpp>

class Intersection
{

public:
	struct CollisionData
	{
		bool collide;
		vec3 normal;
		float profondeur;
		std::vector<glm::vec3> contacts;
	};
	// --- Is Intersection ---
	static bool isIntersectionAABBtoAABB(BoundingBox* a, BoundingBox* b)
	{
		glm::vec3 aMin = a->getMin();
		glm::vec3 aMax = a->getMax();
		glm::vec3 bMin = b->getMin();
		glm::vec3 bMax = b->getMax();

		return aMin.x <= bMax.x && aMax.x >= bMin.x &&
			aMin.y <= bMax.y && aMax.y >= bMin.y &&
			aMin.z <= bMax.z && aMax.z >= bMin.z;
	}

	static bool isIntersectionAABBtoModele(BoundingBox* a, ModeleCollision* b)
	{
		return false;
	}

	// --- Intersection Points ---
	static void intersectionAABBtoAABB(BoundingBox* a, BoundingBox* b, CollisionData* data)
	{
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

	static bool isInTriangle(glm::vec3 p, ModeleCollision::Triangle t, glm::vec3 normT)
	{
		float u0 = glm::dot(normT, glm::cross(t.y - t.x, p - t.x));
		float u1 = glm::dot(normT, glm::cross(t.z - t.y, p - t.y));
		float u2 = glm::dot(normT, glm::cross(t.x - t.z, p - t.z));
		return ((u0 > 0) && (u1 > 0) && (u2 > 0));
	}

	static void intersectionModeleToModele(ModeleCollision* a, ModeleCollision* b, CollisionData* data)
	{
		std::vector<glm::vec3> contacts;
		std::vector<ModeleCollision::Triangle> tA = a->getTriangle();
		std::vector<ModeleCollision::Triangle> tB = b->getTriangle();

		std::cout << "A\n";
		for (size_t i = 0, maxA = tA.size(); i < maxA; i++)
		{
			std::cout << "I : " << i << std::endl;

			for (size_t j = 0, maxB = tB.size(); j < maxB; j++)
			{

				std::cout << "I : " << i << " | J : " << j << std::endl;

				glm::vec3 normA = glm::triangleNormal(tA[i].x, tA[i].y, tA[i].z);
				glm::vec3 normB = glm::triangleNormal(tB[j].x, tB[j].y, tB[j].z);

				bool isInContact = false;
				std::vector<glm::vec3> listPa;
				std::vector<glm::vec3> listPb;


				if (normA == glm::abs(normB))
				{
					std::cout << "B1" << std::endl;
					listPa.push_back(tA[i].x);
					listPa.push_back(tA[i].y);
					listPa.push_back(tA[i].z);
					listPb.push_back(tB[j].x);
					listPb.push_back(tB[j].y);
					listPb.push_back(tB[j].z);
				}
				else
				{
					std::cout << "B2" << std::endl;

					std::vector<glm::vec3> tmpA;
					std::vector<glm::vec3> tmpB;
					tmpA.push_back(tA[i].x - tA[i].y);
					tmpA.push_back(tA[i].y - tA[i].z);
					tmpA.push_back(tA[i].z - tA[i].x);
					tmpB.push_back(tB[j].x - tB[j].y);
					tmpB.push_back(tB[j].y - tB[j].z);
					tmpB.push_back(tB[j].z - tB[j].x);
					for (int k = 0; k < 3; k++)
					{
						std::cout << "B2 A k :" << k << std::endl;
						if (glm::dot(tmpA[k], normB) != 0)
						{
							float t = (glm::dot(tB[j].x - tA[i].get(k), normB)) / glm::dot(tmpA[k], normB);
							listPa.push_back(tA[i].get(k) + t * tmpA[k]);
						}
						if (glm::dot(tmpB[k], normA) != 0)
						{
							float t = (glm::dot(tA[i].x - tB[j].get(k), normA)) / glm::dot(tmpB[k], normA);
							listPb.push_back(tB[j].get(k) + t * tmpB[k]);
						}
					}
				}
				for (int k = 0, maxSize = listPa.size(); k < maxSize && !isInContact; k++)
				{
					std::cout << "C k :"  << k << std::endl;
					isInContact = isInTriangle(listPa[k], tB[j], normB);
				}

				for (int k = 0, maxSize = listPb.size(); k < maxSize && !isInContact; k++)
				{
					std::cout << "D k :" << k << std::endl;
					isInContact = isInTriangle(listPb[k], tA[i], normA);
				}
				std::cout << "E"<< std::endl;

				if (isInContact)
				{
					contacts.push_back(tA[i].x);
					contacts.push_back(tA[i].y);
					contacts.push_back(tA[i].z);
					contacts.push_back(tB[j].x);
					contacts.push_back(tB[j].y);
					contacts.push_back(tB[j].z);
				}
				std::cout << "F" << std::endl;
			}
			std::cout << "G" << std::endl;
		}
		std::cout << "Z" << std::endl;

		if (contacts.size() > 0)
		{
			data->collide = true;
		}
		data->profondeur = 0.0f;
		data->contacts = contacts;
	}

	static void intersectionAABBtoModele(BoundingBox* a, ModeleCollision* b, CollisionData* data)
	{
		std::vector<glm::vec3> contacts;
		std::vector<glm::vec3> cB = b->getCoords();

		float dist = 0.0f;
		for (size_t i = 0, max = cB.size(); i < max; i++)
		{
			dist = glm::max(a->inCollision(cB[i]), dist);
			if (dist >= 0.0f)
			{
				contacts.push_back(cB[i]);
			}
		}

		if (contacts.size() > 0)
		{
			data->collide = true;
		}
		data->profondeur = dist;
		data->contacts = contacts;
	}

	static CollisionData intersection(Collision* c1, Collision* c2, bool minimal = false)
	{
		CollisionData result;
		if (c1->getType() == Collision::BOUNDING_BOX && c2->getType() == Collision::BOUNDING_BOX)
		{
			BoundingBox* a = (BoundingBox*)c1;
			BoundingBox* b = (BoundingBox*)c2;
			result.collide = isIntersectionAABBtoAABB(a, b);
			if (result.collide && !minimal)
			{
				intersectionAABBtoAABB(a, b, &result);
				result.normal = c2->getCenter() - c1->getCenter();
			}
		}
		else if (c1->getType() == Collision::MODELE && c2->getType() == Collision::MODELE)
		{
			std::cout << "MODELE VS MODELE\n";
			ModeleCollision* a = (ModeleCollision*)c1;
			ModeleCollision* b = (ModeleCollision*)c2;
			intersectionModeleToModele(a, b, &result);
			std::cout << "END VS\n";
		}
		else if (c1->getType() == Collision::BOUNDING_BOX && c2->getType() == Collision::MODELE)
		{
			BoundingBox* a = (BoundingBox*)c1;
			ModeleCollision* b = (ModeleCollision*)c2;
			result.collide = isIntersectionAABBtoModele(a, b);
			if (result.collide && !minimal)
			{
				intersectionAABBtoModele(a, b, &result);
			}
		}
		else if (c1->getType() == Collision::MODELE && c2->getType() == Collision::BOUNDING_BOX)
		{
			BoundingBox* a = (BoundingBox*)c1;
			ModeleCollision* b = (ModeleCollision*)c2;
			result.collide = isIntersectionAABBtoModele(a, b);
			if (result.collide && !minimal)
			{
				intersectionAABBtoModele(a, b, &result);
			}
		}
		return result;
	}
};
#endif // !__INTERSECTION_HPP__
