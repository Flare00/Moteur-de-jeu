#ifndef __PHYSIQUE_HPP__
#define __PHYSIQUE_HPP__

#include <vector>
#include <Component.hpp>
#include "RigidBody.hpp"

class Physique {
public:
	static void applyImpulse(RigidBody * r1, RigidBody * r2){
		float iMassR1 = r1->getInvMass();
		float iMassR2 = r2->getInvMass();
		float iMassSum = iMassR1 + iMassR2;
		if(iMassSum == 0.0f) return;
		
		glm::vec3 vitesseR = r1->computeVitesseRelative(r2);
		glm::vec3 normalR = glm::normalize(r2->getCollision()->getCenter() - r1->getCollision()->getCenter());

		float dotR = glm::dot(vitesseR, normalR);
		if(dotR > 0.0f) return;

		float e = (r1->getCoefRestitution() < r2->getCoefRestitution() ? r1->getCoefRestitution() : r2->getCoefRestitution());
		float num = -(1.0f + e) * dotR;
		float j = num / iMassSum;

		glm::vec3 impulse = vitesseR * j;
		r1->addForces(impulse);
		r2->addForces(-impulse);
		//glm::vec3 t = vitesseR - (normalR * dotR);

	}

	static void computePhysique(GameObject* scene, float delta) {
		std::vector<GameObject*> rigidbodiesGO = scene->getAllGameObjectByComponentType(Component::Type::RIGIDBODY);
		std::vector<Component*> rigidbodies = scene->getAllComponentsByTypeRecursive(Component::Type::RIGIDBODY);

		for (int i = 0, max = rigidbodiesGO.size(); i < max;  i++) {
			for (int j = i + 1; j < max; j++) {
				RigidBody* r1 = (RigidBody*)rigidbodiesGO[i]->getOneComponentByType(Component::Type::RIGIDBODY);
				RigidBody* r2 = (RigidBody*)rigidbodiesGO[j]->getOneComponentByType(Component::Type::RIGIDBODY);
				r1->getCollision()->apply(rigidbodiesGO[i]->getTransformMatrix());
				r2->getCollision()->apply(rigidbodiesGO[j]->getTransformMatrix());
				if (Collision::computeCollision(r1->getCollision(), r2->getCollision())) {
					applyImpulse(r1,r2);
				}
			}
		}

		for (int i = 0, max = rigidbodies.size(); i < max; i++) {
			((RigidBody*)rigidbodies[i])->applyVitesse(delta, 0.98f);
		}
	}
};

#endif // !__PHYSIQUE_HPP__
