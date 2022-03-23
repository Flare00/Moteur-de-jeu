#ifndef __PHYSIQUE_HPP__
#define __PHYSIQUE_HPP__

#include <vector>
#include "Collision.hpp"
#include "Gravity.hpp"

class Physique {
public:
	static void computePhysique(GameObject* scene, float delta) {
		std::vector<GameObject*> collisionGO = scene->getAllGameObjectByComponentType(Component::Type::Collision);
		std::vector<Component*> gravitys = scene->getAllComponentsByTypeRecursive(Component::Type::Gravity);

		for (int i = 0, max = collisionGO.size(); i < max;  i++) {
			for (int j = i + 1; j < max; j++) {
				Collision* c1 = (Collision*)collisionGO[i]->getOneComponentByType(Component::Type::Collision);
				Collision* c2 = (Collision*)collisionGO[j]->getOneComponentByType(Component::Type::Collision);
				c1->apply(collisionGO[i]->getTransformMatrix());
				c2->apply(collisionGO[j]->getTransformMatrix());
				if (Collision::computeCollision(c1, c2)) {
					Gravity* g1 = (Gravity*)collisionGO[i]->getOneComponentByType(Component::Type::Gravity);
					Gravity* g2 = (Gravity*)collisionGO[j]->getOneComponentByType(Component::Type::Gravity);
					if (g1 != NULL) {
						g1->setCollision(true);
					}
					if (g2 != NULL) {
						g2->setCollision(true);
					}
				}
			}
		}

		for (int i = 0, max = gravitys.size(); i < max; i++) {
			((Gravity*) gravitys[i])->apply(delta);
			((Gravity*) gravitys[i])->setCollision(false);
		}
	}
};

#endif // !__PHYSIQUE_HPP__
