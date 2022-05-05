#ifndef __COLLISION_COMPONENT_HPP__
#define __COLLISION_COMPONENT_HPP__
#include <typeinfo>

#include <Component.hpp>
#include <Collision/Type/Intersection.hpp>
#include <Collision/Type/Collision.hpp>
#include <Collision/Type/BoundingBox.hpp>
#include <Collision/Type/ModeleCollision.hpp>

class CollisionComponent : public Component
{
public :
	struct CollisionData {
		bool collide;
		vec3 normal;
		float profondeur;
		std::vector<glm::vec3> contacts;
	};

private:
	Collision *collision = NULL;
	bool actif;

public:
	CollisionComponent(bool actif = false) : Component(Component::Type::COLLISION)
	{
		this->actif = actif;
	}
	//Bounding Box
	CollisionComponent(std::vector<glm::vec3> vertices, bool actif = true) : Component(Component::Type::COLLISION)
	{
		this->collision = new BoundingBox(vertices);
	}
	//Modele Collision
	CollisionComponent(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, bool actif = true) : Component(Component::Type::COLLISION)
	{
		this->collision = new ModeleCollision(vertices, indices);
	}
	//Sphere Collision
	CollisionComponent(glm::vec3 center, float radius, bool actif = true) : Component(Component::Type::COLLISION)
	{
		this->collision = new SphereCollision(center, radius);
	}

	~CollisionComponent()
	{
		delete this->collision;
	}

	static std::vector<glm::vec3> getContactPoint(CollisionComponent *c1, CollisionComponent *c2)
	{
		std::vector<glm::vec3> res;

		res = Intersection::intersectionPoints(c1->getCollision(), c2->getCollision());

		return res;
	}

	static bool computeCollision(CollisionComponent *c1, CollisionComponent *c2)
	{
		bool res = false;
		if (c1->isActif() && c2->isActif())
		{
			if (Intersection::isIntersection(c1->getCollision(), c2->getCollision()))
			{
				// do phase 2
				res = true;
			}
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

	Collision *getCollision()
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

	void setCollision(Collision *c)
	{
		this->collision = c;
	}

	virtual void apply(glm::mat4 tranformationMatrix)
	{
		this->collision->applyTransformation(tranformationMatrix);
	}
};

#endif