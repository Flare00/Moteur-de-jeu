#ifndef __BULLET_RIGIDBODY_HPP__
#define __BULLET_RIGIDBODY_HPP__

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <Physique/SwapGLM_BT.hpp>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

class BulletRigidbody {
public:
	enum Type {
		AUCUN,
		AABB,
		SPHERE,
		CAPSULE,
		CYLINDER,
		HEIGHT_TERRAIN
	};
protected:
	Type type = Type::AUCUN;
	btRigidBody* rigidbody = NULL;
	btCollisionShape* shape;
public:
	BulletRigidbody() {
	}

	~BulletRigidbody() {
		delete rigidbody;
		delete shape;
	}

	void setToAABB(glm::vec3 halfSize, glm::vec3 origine, float masse) {
		this->type = Type::AABB;
		shape = new btBoxShape(swapGlmBt(halfSize));
		setToShape(this->shape, origine, masse);
	}

	void setToSphere(float radius, glm::vec3 origine, float masse) {
		this->type = Type::SPHERE;
		this->shape = new btSphereShape(radius);
		setToShape(this->shape, origine, masse);
	}

	void setToCapsule(float radius, float height, glm::vec3 origine, float masse) {
		this->type = Type::CAPSULE;
		this->shape = new btCapsuleShape(radius, height);
		setToShape(this->shape, origine, masse);
	}

	void setToCylinder(glm::vec3 halfSize, glm::vec3 origine, float masse) {
		this->type = Type::CYLINDER;
		this->shape = new btCylinderShape(swapGlmBt(halfSize));
		setToShape(this->shape, origine, masse);
	}

	/*void setToHeightTerrain(glm::vec3 halfSize, glm::vec3 origine) {
		this->type = Type::HEIGHT_TERRAIN;
		this->shape = new btHeightfieldTerrainShape();
		setToShape(this->shape, origine, 0.0f);
	}*/

	void setToShape(btCollisionShape* shape, glm::vec3 origine, float masse) {
		btTransform transform = btTransform::getIdentity();
		transform.setOrigin(swapGlmBt(origine));
		this->rigidbody = createRigidBody(masse, transform, shape);
	}

	btRigidBody* getRigidbody() {
		return this->rigidbody;
	}

	btCollisionShape* getShape() {
		return this->shape;
	}

	Type getType() {
		return this->type;
	}

protected:
	btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4& color = btVector4(1, 0, 0, 1))
	{
		btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			shape->calculateLocalInertia(mass, localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

		btRigidBody* body = new btRigidBody(cInfo);
		//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);


		body->setUserIndex(-1);
		return body;
	}

};

#endif // !__BULLET_RIGIDBODY__
