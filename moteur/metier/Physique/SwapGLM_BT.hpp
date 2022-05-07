#ifndef __SWAP_GLM_BT_HPP__
#define __SWAP_GLM_BT_HPP__
#include <glm/glm.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

btVector3 swapGlmBt(glm::vec3 a) {
	return btVector3(a.x, a.y, a.z);
}

glm::vec3 swapGlmBt(btVector3 a) {
	return glm::vec3(a.getX(), a.getY(), a.getZ());
}

#endif // !__SWAP_GLM_BT_HPP__
