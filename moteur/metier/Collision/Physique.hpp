#ifndef __PHYSIQUE_HPP__
#define __PHYSIQUE_HPP__

#include <vector>
#include <Component.hpp>
#include "RigidBody.hpp"

#define EPSILON 0.00001f

class Physique
{
public:
	static bool compareEpsilon(float x)
	{
		return abs(x) <= EPSILON * glm::max(abs(1.0f), abs(x));
	}

	static void applyImpulse(RigidBody *r1, RigidBody *r2)
	{
		// MASS
		float iMassR1 = r1->getInvMass();
		float iMassR2 = r2->getInvMass();
		float iMassSum = iMassR1 + iMassR2;
		if (iMassSum == 0.0f)
			return;

		glm::vec3 vitesseR = r1->computeVitesseRelative(r2);
		glm::vec3 normalR = glm::normalize(r2->getCollision()->getCenter() - r1->getCollision()->getCenter());

		float dotR = glm::dot(vitesseR, normalR);
		if (dotR > 0.0f)
			return;

		float e = glm::min(r1->getCoefRestitution(), r2->getCoefRestitution());
		float numerator = -(1.0f + e) * dotR;
		float magnitude = numerator / iMassSum;

		std::vector<glm::vec3> ptsCollide = Collision::getContactPoint(r1->getCollision(), r2->getCollision());
		if (ptsCollide.size() > 0 && magnitude != 0.0f)
		{
			magnitude /= (float)ptsCollide.size();
		}

		glm::vec3 impulse = normalR * magnitude;
		
		/*
		r1->addForces(-impulse);
		r2->addForces(impulse);
		*/
		
		r1->setVitesse(r1->getVitesse() - impulse * iMassR1);
		r2->setVitesse(r2->getVitesse() + impulse * iMassR2);

		// STOP FORCES
		/*
		if (r1->getMass() < r2->getMass())
		{
			r1->setVitesse(glm::vec3(0.0f));
		}
		else if (r1->getMass() > r2->getMass())
		{
			r2->setVitesse(glm::vec3(0.0f));
		}
		else
		{
			r1->setVitesse(glm::vec3(0.0f));
			r2->setVitesse(glm::vec3(0.0f));
		}
		*/

		glm::vec3 t = vitesseR - (normalR * dotR);
		float dotT = glm::dot(t, t);
		if (compareEpsilon(dotT))
		{
			return;
		}
		t = glm::normalize(t);

		numerator = -glm::dot(vitesseR, t);
		float jt = numerator / iMassSum;
		if (ptsCollide.size() > 0 && jt != 0.0f)
		{
			jt /= (float)ptsCollide.size();
		}
		if (compareEpsilon(jt))
		{
			return;
		}

		// COULOMBS
		float friction = glm::sqrt(r1->getFriction() * r2->getFriction());
		if (jt > magnitude * friction)
		{
			jt = magnitude * friction;
		}
		else if (jt < -magnitude * friction)
		{
			jt = -magnitude * friction;
		}

		glm::vec3 tangentImpuse = t * jt;
		r1->addForces(-tangentImpuse);
		r2->addForces(tangentImpuse);
	}

	static void computePhysique(GameObject *scene, float delta)
	{
		std::vector<GameObject *> rigidbodiesGO = scene->getAllGameObjectByComponentType(Component::Type::RIGIDBODY);
		std::vector<Component *> rigidbodies = scene->getAllComponentsByTypeRecursive(Component::Type::RIGIDBODY);

		for (int i = 0, max = rigidbodiesGO.size(); i < max; i++)
		{
			for (int j = i + 1; j < max; j++)
			{
				RigidBody *r1 = (RigidBody *)rigidbodiesGO[i]->getOneComponentByType(Component::Type::RIGIDBODY);
				RigidBody *r2 = (RigidBody *)rigidbodiesGO[j]->getOneComponentByType(Component::Type::RIGIDBODY);
				r1->getCollision()->apply(rigidbodiesGO[i]->getTransformMatrix());
				r2->getCollision()->apply(rigidbodiesGO[j]->getTransformMatrix());
				if (Collision::computeCollision(r1->getCollision(), r2->getCollision()))
				{
					applyImpulse(r1, r2);
				}
			}
		}

		for (int i = 0, max = rigidbodies.size(); i < max; i++)
		{
			((RigidBody *)rigidbodies[i])->applyVitesse(delta, 1.0f);
		}
	}
};

#endif // !__PHYSIQUE_HPP__
