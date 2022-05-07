#ifndef __FRUSTUM_HPP__
#define __FRUSTUM_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Collision/Type/BoundingBox.hpp>
#include <Collision/Type/SphereCollision.hpp>

#include <btBulletCollisionCommon.h>

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Physique/BulletRigidbody.hpp>
#include <BulletCollision/Gimpact/btBoxCollision.h>

using namespace glm;
class Frustum {
	btPairCachingGhostObject* bulletFrustum;
	mat4 matrix = mat4(1.0f);
	float marge = 0.1f;
public:
	Frustum() {
	}

	void init(mat4 proj, mat4 view) {
		this->matrix = proj * view;
		glm::mat4 inverse = glm::inverse(this->matrix);
		vec4 f[8] = {
			//near
			vec4(1, 1,-1, 1),
			vec4(-1, 1,-1, 1),
			vec4(1,-1,-1, 1),
			vec4(-1,-1,-1, 1),
			//far
			vec4(1, 1, 1, 1),
			vec4(-1, 1, 1, 1),
			vec4(1,-1, 1, 1),
			vec4(-1,-1, 1, 1),
		};
		bulletFrustum = new btPairCachingGhostObject();
		btConvexHullShape* hull = new btConvexHullShape();
		for (int i = 0; i < 8; i++) {
			vec4 v = inverse * f[i];
			hull->addPoint(btVector3(v.x / v.w, v.y / v.w, v.z / v.w));
		}
		bulletFrustum->setCollisionShape(hull);
		bulletFrustum->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		bulletFrustum->setUserIndex(-1);
	}



	void update(mat4 proj, mat4 view, vec3 pos) {
		this->matrix = proj * view;
		glm::vec3 fpos = swapGlmBt(bulletFrustum->getWorldTransform().getOrigin());
		bulletFrustum->getWorldTransform().setOrigin(swapGlmBt(pos));
		btMatrix3x3 rota(-view[0][0], -view[0][1], -view[0][2], -view[1][0], -view[1][1], -view[1][2], -view[2][0], -view[2][1], -view[2][2]);
		bulletFrustum->getWorldTransform().setBasis(rota);
	}

	bool isVisible(vec3 a) {
		vec4 p = this->matrix * vec4(a, 1.0f);
		return	abs(p.x) < p.w &&
			abs(p.y) < p.w &&
			p.z > -marge &&
			p.z < p.w + marge;

	}

	bool isVisible(BulletRigidbody* a) {
		bool res = false;

		// Je ne sais pas

		//bottom
		/*res = isVisible(swapGlmBt(min));
		if (!res) res = isVisible(glm::vec3(max.getX(), min.getY(), min.getZ()));
		if (!res) res = isVisible(glm::vec3(max.getX(), min.getY(), max.getZ()));
		if (!res) res = isVisible(glm::vec3(min.getX(), min.getY(), max.getZ()));
		//top
		if (!res) res = isVisible(swapGlmBt(max));
		if (!res) res = isVisible(glm::vec3(max.getX(), max.getY(), min.getZ()));
		if (!res) res = isVisible(glm::vec3(max.getX(), max.getY(), max.getZ()));
		if (!res) res = isVisible(glm::vec3(min.getX(), max.getY(), max.getZ()));*/
		
		return false;
	}

	bool isVisible(SphereCollision* a) {
		vec4 center = vec4(a->getCenter(), 1.0f);
		float rayon = a->getRadius();

		float w_rayon = center.w + rayon;

		return abs(center.x) < w_rayon &&
			abs(center.y) < w_rayon &&
			center.z > -rayon &&
			center.z < w_rayon;
	}

	btPairCachingGhostObject* getBulletFrustum() {
		return this->bulletFrustum;
	}
};

#endif // !__FRUSTUM_HPP__


