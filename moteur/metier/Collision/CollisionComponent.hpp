#ifndef __COLLISION_COMPONENT_HPP__
#define __COLLISION_COMPONENT_HPP__
#include <typeinfo>

#include <Component.hpp>
#include <Collision/Type/Intersection.hpp>
#include <Collision/Type/Collision.hpp>
#include <Collision/Type/BoundingBox.hpp>
#include <Collision/Type/ModeleCollision.hpp>
#include <Collision/Type/TerrainCollision.hpp>
#include <GameObject/ModeleComponent.hpp>
class CollisionComponent : public Component
{
public:


private:
	Collision* collision = NULL;
	bool actif;

public:
	CollisionComponent(bool actif = false) : Component(Component::Type::COLLISION)
	{
		this->actif = actif;
	}
	//Bounding Box
	CollisionComponent(std::vector<glm::vec3> vertices, bool actif = true) : Component(Component::Type::COLLISION)
	{
		this->actif = actif;
		this->collision = new BoundingBox(vertices);
	}
	//Modele Collision
	CollisionComponent(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, bool actif = true) : Component(Component::Type::COLLISION)
	{
		this->actif = actif;
		this->collision = new ModeleCollision(vertices, indices);
	}
	//Sphere Collision
	CollisionComponent(glm::vec3 center, float radius, bool actif = true) : Component(Component::Type::COLLISION)
	{
		this->actif = actif;
		this->collision = new SphereCollision(center, radius);
	}
	//Terrain Collision
	CollisionComponent(Texture* texture, std::vector<glm::vec3> vertex, float height, bool actif = true) : Component(Component::Type::COLLISION)
	{
		this->actif = actif;
		this->collision = new TerrainCollision(texture, vertex, height);
	}

	~CollisionComponent()
	{
		delete this->collision;
	}

	static float computeZDistanceToTerrain(CollisionComponent* c1, CollisionComponent* c2) {
		float res = FLT_MAX;
		if (c1->getCollision()->getType() == Collision::TERRAIN && c2->getCollision()->getType() == Collision::TERRAIN) {
			return -res;
		}
		TerrainCollision* terrain = NULL;
		Collision* other = NULL;
		if (c1->getCollision()->getType() == Collision::TERRAIN)
			terrain = (TerrainCollision*)c1->getCollision();

		if (terrain == NULL) {
			if (c2->getCollision()->getType() == Collision::TERRAIN)
				terrain = (TerrainCollision*)c2->getCollision();

			if (terrain == NULL) return -res;
			other = c1->getCollision();
		}
		else {
			other = c2->getCollision();
		}

		if (other->getType() == Collision::BOUNDING_BOX) {
			std::cout << "AABB\n";
			BoundingBox* a = (BoundingBox*)other;
			glm::vec3 center = a->getCenter();
			
			float dMin = terrain->distanceZTerrain(glm::vec3(center.x, a->getMin().y, center.z));
			float dMax = terrain->distanceZTerrain(glm::vec3(center.x, a->getMax().y, center.z));
			if (glm::abs(dMin) < glm::abs(dMax)) {
				res = dMin;
			}
			else {
				res = dMax;
			}
		}
		else if (other->getType() == Collision::SPHERE) {
			std::cout << "SPHERE\n";

			SphereCollision* a = (SphereCollision*)other;
			float dist = terrain->distanceZTerrain(a->getCenter());
			if (dist < 0) {
				dist += a->getRadius();
			}
			else {
				dist -= a->getRadius();
			}
		}
		else if (other->getType() == Collision::MODELE) {
			std::cout << "MODELE\n";

			ModeleCollision* a = (ModeleCollision*)other;

			//TODO
		}

		return res;

	}

	static Intersection::CollisionData computeCollision(CollisionComponent* c1, CollisionComponent* c2)
	{
		Intersection::CollisionData res;
		res.collide = false;
		if (c1->isActif() && c2->isActif())
		{
			res = Intersection::intersection(c1->getCollision(), c2->getCollision());
		}
		return res;
	}
	static Intersection::CollisionData isCollision(CollisionComponent* c1, CollisionComponent* c2)
	{
		Intersection::CollisionData res;
		res.collide = false;
		if (c1->isActif() && c2->isActif())
		{
			res = Intersection::intersection(c1->getCollision(), c2->getCollision());
		}
		return res;
	}

	// Getter ans Setter
	bool isActif()
	{
		return this->actif;
	}

	void setActif(bool a)
	{
		this->actif = a;
	}

	Collision* getCollision()
	{
		return this->collision;
	}

	std::vector<glm::vec3> getCoords() {
		return this->collision->getCoords();
	}

	glm::vec3 getCenter()
	{
		return this->collision->getCenter();
	}

	void setCollision(Collision* c)
	{
		this->collision = c;
	}

	virtual void apply(glm::mat4 tranformationMatrix)
	{
		this->collision->applyTransformation(tranformationMatrix);
	}
};

#endif