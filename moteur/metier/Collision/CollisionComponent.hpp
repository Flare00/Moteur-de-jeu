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

	static Intersection::CollisionData computeCollision(CollisionComponent *c1, CollisionComponent *c2)
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