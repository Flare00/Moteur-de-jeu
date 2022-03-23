#ifndef __PHYSIQUE_HPP__
#define __PHYSIQUE_HPP__
#include <vector>
#include "Collision.hpp";

class Physique {
public:
	static void computeCollision(GameObject* scene) {
		std::vector<Collision*> compo = scene->getAllComponentsByTypeRecursive(Collision::Type::Collision);

		for (int i = 0, max = compo.size(); i++) {
			for (int j = i + 1; j < max; j++) {
				bool ah = Collision::computeCollision(compo[i], compo[j]);
			}
		}
	}
	static void computePhysique()
};

#endif // !__PHYSIQUE_HPP__
