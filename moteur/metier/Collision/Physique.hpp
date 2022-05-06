#ifndef __PHYSIQUE_HPP__
#define __PHYSIQUE_HPP__

#include <vector>
#include <Component.hpp>
#include "RigidBody.hpp"
#include <Collision/CollisionComponent.hpp>
#include <Collision/Type/Intersection.hpp>

#define EPSILON 0.00001f

glm::vec3 GRAVITY = glm::vec3(0, -9.8, 0);

class Physique
{
public:
	static bool compareEpsilon(float x)
	{
		return abs(x) <= EPSILON * glm::max(abs(1.0f), abs(x));
	}

	static void applyImpulse(RigidBody* r1, RigidBody* r2, Intersection::CollisionData data)
	{
		// MASS
		float iMassR1 = r1->getInvMass();
		float iMassR2 = r2->getInvMass();
		float iMassSum = iMassR1 + iMassR2;
		if (iMassSum == 0.0f)
			return;

		glm::vec3 vitesseR = r1->computeVitesseRelative(r2);

		glm::vec3 normalR = glm::normalize(data.normal);



		float dotR = glm::dot(vitesseR, normalR);

		if (dotR > 0.0f)
			return;

		float e = glm::min(r1->getCoefRestitution(), r2->getCoefRestitution());
		float numerator = -(1.0f + e) * dotR;
		float magnitude = numerator / iMassSum;

		std::vector<glm::vec3> ptsCollide = data.contacts;
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

	static void computePhysique(GameObject* scene, float delta)
	{
		std::vector<GameObject*> rigidbodiesGO = scene->getAllGameObjectByComponentType(Component::Type::RIGIDBODY);
		std::vector<Component*> rigidbodies = scene->getAllComponentsByTypeRecursive(Component::Type::RIGIDBODY);

		for (size_t i = 0, max = rigidbodiesGO.size(); i < max; i++)
		{
			for (size_t j = i + 1; j < max; j++)
			{
				RigidBody* r1 = (RigidBody*)rigidbodiesGO[i]->getOneComponentByType(Component::Type::RIGIDBODY);
				RigidBody* r2 = (RigidBody*)rigidbodiesGO[j]->getOneComponentByType(Component::Type::RIGIDBODY);
				r1->getCollision()->apply(rigidbodiesGO[i]->getTransformMatrix());
				r2->getCollision()->apply(rigidbodiesGO[j]->getTransformMatrix());
				if (r1->getCollision()->getCollision()->getType() == Collision::TERRAIN || r2->getCollision()->getCollision()->getType() == Collision::TERRAIN) {
					std::cout << "TERRAIN\n";
					float dist = CollisionComponent::computeZDistanceToTerrain(r1->getCollision(), r2->getCollision());
					std::cout << "Dist : " << dist << std::endl;

					if (glm::abs(dist) < 0.05f) {
						if (r1->getCollision()->getCollision()->getType() == Collision::TERRAIN) {
							glm::vec3 vitesse = r2->getVitesse();
							vitesse.y = 0;
							r2->setVitesse(vitesse);
						}
						else {
							glm::vec3 vitesse = r1->getVitesse();
							vitesse.y = 0;
							r1->setVitesse(vitesse);
						}
					}
				}
				else {

					Intersection::CollisionData data;
					data = CollisionComponent::computeCollision(r1->getCollision(), r2->getCollision());
					/* // beaucoup trop gourmand
					if ((r1->hasModeleCollision() || r2->hasModeleCollision()) && data.collide)
					{
						std::cout << "Modele" << std::endl;
						CollisionComponent *c1;
						CollisionComponent *c2;
						if (r1->hasModeleCollision())
						{
							c1 = r1->getCollisionModele();
						}
						else
						{
							c1 = r1->getCollision();
						}
						if (r2->hasModeleCollision())
						{
							c2 = r2->getCollisionModele();
						}
						else
						{
							c2 = r2->getCollision();
						}
						data = CollisionComponent::computeCollision(c1, c2);
					}*/

					if (data.collide)
					{
						for (int k = 0; k < 10; k++)
						{
							applyImpulse(r1, r2, data);
						}

						float invMass = (r1->getInvMass() + r2->getInvMass());
						if (invMass != 0.0f)
						{
							float precision = 0.01f;
							std::cout << data.profondeur << std::endl;
							float distance = glm::max(data.profondeur + precision, 0.0f);

							float scalar = distance / invMass;
							glm::vec3 correction = data.normal * scalar * 0.45f;
							r1->addCorrection(-correction);
							r2->addCorrection(correction);

							// rigidbodiesGO[i]->getTransform()->translate(data.normal * (data.profondeur / (r2->getMass() + r1->getMass()) * 0.45f));
							// rigidbodiesGO[j]->getTransform()->translate(-data.normal * (data.profondeur / (r2->getMass() + r1->getMass()) * 0.45f));
						}
					}
				}
			}
		}

		for (size_t i = 0, max = rigidbodies.size(); i < max; i++)
		{
			RigidBody* r = (RigidBody*)rigidbodies[i];
			r->addForces(GRAVITY);
			r->applyVitesse(delta, 1.0f - (0.02f * delta));
		}
	}
};

#endif // !__PHYSIQUE_HPP__
