#ifndef __BULLET_RIGIDBODY_HPP__
#define __BULLET_RIGIDBODY_HPP__
#include <cstdlib>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include <Texture.hpp>

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

	void setToAABB(glm::vec3 halfSize, float masse) {
		this->type = Type::AABB;
		shape = new btBoxShape(btVector3(halfSize.x, halfSize.y, halfSize.z));
		setToShape(this->shape, masse);
	}

	void setToSphere(float radius, float masse) {
		this->type = Type::SPHERE;
		this->shape = new btSphereShape(radius);
		setToShape(this->shape, masse);
	}

	void setToCapsule(float radius, float height, float masse) {
		this->type = Type::CAPSULE;
		this->shape = new btCapsuleShape(radius, height);
		setToShape(this->shape, masse);
	}

	void setToCylinder(glm::vec3 halfSize, float masse) {
		this->type = Type::CYLINDER;
		this->shape = new btCylinderShape(btVector3(halfSize.x, halfSize.y, halfSize.z));
		setToShape(this->shape, masse);
	}

	void setToHeightTerrain(Texture* texture, float maxHeight, float widthObject) {


		int width = texture->getWidth();
		int height = texture->getHeight();
		float scale = widthObject / width;
		unsigned char* textData = texture->getData();

		float* data = new float[width * height];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				data[(i * width) + j] = ((float)textData[((i * width) + (width - 1 - j))*4]) / 255.0f;
			}
		}

		this->type = Type::HEIGHT_TERRAIN;

		this->shape = new btHeightfieldTerrainShape(width, height, (const float*)data, btScalar(0), btScalar(1), 1, false);
		this->shape->setLocalScaling(btVector3(scale, maxHeight, scale));
		setToShape(this->shape, 0.0f);
	}

	void setToShape(btCollisionShape* shape, float masse) {
		btTransform transform = btTransform::getIdentity();
		transform.setOrigin(btVector3(0, 0, 0));
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
