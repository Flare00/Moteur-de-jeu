#ifndef __BULLET_RIGIDBODY_HPP__
#define __BULLET_RIGIDBODY_HPP__
#include <cstdlib>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionShapes/btEmptyShape.h>

#include <Texture.hpp>

class BulletRigidbody
{
public:
	enum Type
	{
		AUCUN,
		AABB,
		SPHERE,
		CAPSULE,
		CYLINDER,
		HEIGHT_TERRAIN
	};

protected:
	Type type = Type::AUCUN;
	btRigidBody *rigidbody = NULL;
	btCollisionShape *shape;
	float masse = 0.0f;
	glm::mat4 modelTransformation;

public:
	BulletRigidbody(glm::mat4 modelMatrix = glm::mat4(1.0f))
	{
		this->modelTransformation = modelMatrix;
	}

	BulletRigidbody(btCollisionShape *b, float m, glm::mat4 modelMatrix = glm::mat4(1.0f))
	{
		this->shape = b;
		this->masse = m;
		this->modelTransformation = modelMatrix;
		setToShape(this->shape, this->masse);
	}

	~BulletRigidbody()
	{
		delete rigidbody;
		delete shape;
	}

	void setToAABB(glm::vec3 halfSize, float masse)
	{
		this->type = Type::AABB;
		shape = new btBoxShape(btVector3(halfSize.x, halfSize.y, halfSize.z));
		setToShape(this->shape, masse);
	}

	void setToSphere(float radius, float masse)
	{
		this->type = Type::SPHERE;
		this->shape = new btSphereShape(radius);
		setToShape(this->shape, masse);
	}

	void setToCapsule(float radius, float height, float masse)
	{
		this->type = Type::CAPSULE;
		this->shape = new btCapsuleShape(radius, height);
		setToShape(this->shape, masse);
	}

	void setToCylinder(glm::vec3 halfSize, float masse)
	{
		this->type = Type::CYLINDER;
		this->shape = new btCylinderShape(btVector3(halfSize.x, halfSize.y, halfSize.z));
		setToShape(this->shape, masse);
	}

	void setToHeightTerrain(Texture *texture, float maxHeight, float widthObject)
	{

		int width = texture->getWidth();
		int height = texture->getHeight();
		float scale = widthObject / width;
		unsigned char *textData = texture->getData();

		float *data = new float[width * height];
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				data[(i * width) + j] = ((float)textData[((i * width) + (width - 1 - j)) * 4]) / 255.0f;
			}
		}

		this->type = Type::HEIGHT_TERRAIN;

		this->shape = new btHeightfieldTerrainShape(width, height, (const float *)data, btScalar(0), btScalar(1), 1, false);
		this->shape->setLocalScaling(btVector3(scale, maxHeight, scale));
		setToShape(this->shape, 0.0f);
	}

	void setToShape(btCollisionShape *shape, float masse)
	{
		this->masse = masse;
		btTransform transform = btTransform::getIdentity();
		transform.setOrigin(btVector3(0, 0, 0));
		this->rigidbody = createRigidBody(masse, transform, shape);
	}

	glm::mat4 getModelTransformation()
	{
		return this->modelTransformation;
	}

	void setModelTransformation(glm::mat4 m)
	{
		this->modelTransformation = m;
	}

	btRigidBody *getRigidbody()
	{
		return this->rigidbody;
	}

	btCollisionShape *getShape()
	{
		return this->shape;
	}

	Type getType()
	{
		return this->type;
	}

	float getMasse()
	{
		return this->masse;
	}

	static BulletRigidbody *generateAABB(glm::vec3 halfSize, float masse, glm::mat4 modelMatrix = glm::mat4(1.0f))
	{
		BulletRigidbody *b = new BulletRigidbody();
		b->setToAABB(halfSize, masse);
		b->setModelTransformation(modelMatrix);
		return b;
	}
	static BulletRigidbody *generateSphere(float radius, float masse, glm::mat4 modelMatrix = glm::mat4(1.0f))
	{
		BulletRigidbody *b = new BulletRigidbody();
		b->setToSphere(radius, masse);
		b->setModelTransformation(modelMatrix);
		return b;
	}
	static BulletRigidbody *generateCapsule(float radius, float height, float masse, glm::mat4 modelMatrix = glm::mat4(1.0f))
	{
		BulletRigidbody *b = new BulletRigidbody();
		b->setToCapsule(radius, height, masse);
		b->setModelTransformation(modelMatrix);
		return b;
	}
	static BulletRigidbody *generateCylinder(glm::vec3 halfSize, float masse, glm::mat4 modelMatrix = glm::mat4(1.0f))
	{
		BulletRigidbody *b = new BulletRigidbody();
		b->setToCylinder(halfSize, masse);
		b->setModelTransformation(modelMatrix);
		return b;
	}
	static BulletRigidbody *generateTerrain(Texture *texture, float maxHeight, float widthObject, glm::mat4 modelMatrix = glm::mat4(1.0f))
	{
		BulletRigidbody *b = new BulletRigidbody();
		b->setToHeightTerrain(texture, maxHeight, widthObject);
		b->setModelTransformation(modelMatrix);
		return b;
	}

	BulletRigidbody *duplicate()
	{
		return duplicate(this);
	}

	static BulletRigidbody *duplicate(BulletRigidbody *b)
	{
		return new BulletRigidbody(b->getShape(), b->getMasse(), b->getModelTransformation());
	}

protected:
	btRigidBody *createRigidBody(float mass, const btTransform &startTransform, btCollisionShape *shape, const btVector4 &color = btVector4(1, 0, 0, 1))
	{
		btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

		// rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(1, 1, 1);
		if (isDynamic)
			shape->calculateLocalInertia(mass, localInertia);

		// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);

		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

		btRigidBody *body = new btRigidBody(cInfo);
		// body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

		body->setUserIndex(-1);
		return body;
	}
};

#endif // !__BULLET_RIGIDBODY__
