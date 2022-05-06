#ifndef __PHYSIQUE_BULLET_HPP__
#define __PHYSIQUE_BULLET_HPP__
/*
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>


#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include "DebugDrawer.hpp"

#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Z 5



class PhysiqueBullet {

public:
	btDiscreteDynamicsWorld* dynamicsWorld;
	btSequentialImpulseConstraintSolver* solver;
	btBroadphaseInterface* overlappingPairCache;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfig;

	btBoxShape* box;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	DebugDrawer debug;

	PhysiqueBullet() {


	}



	void init() {
		//Mise en place de la configuration pour collision
		collisionConfig = new btDefaultCollisionConfiguration();
		//Mono Thread dispatcher (Regarder les BulletMultiThreaded pour le multi Thread)
		dispatcher = new btCollisionDispatcher(collisionConfig);
		//Overlapping broadphase , on peut essayer btAxis3Sweep
		overlappingPairCache = new btDbvtBroadphase();
		//Constraint solver (peut être multi thread, même chose que le dispatcher)
		solver = new btSequentialImpulseConstraintSolver();

		//Genere le monde dynamique
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
		//Ajoute la gravité
		dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

		this->debug.DBG_DrawWireframe;
		this->debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe);

		dynamicsWorld->setDebugDrawer(&this->debug);
		//End Init
		box = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
		collisionShapes.push_back(box);


		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -50, 0));
		btScalar mass(0.);
		createRigidBody(mass, groundTransform, box, btVector4(0, 0, 1, 1));

		btBoxShape* colShape = new btBoxShape(btVector3(.1, .1, .1));
		collisionShapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setIdentity();
		btScalar mass2(1.f);

		bool isDynamic = (mass2 != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass2, localInertia);

		for (int k = 0; k < ARRAY_SIZE_Y; k++)
		{
			for (int i = 0; i < ARRAY_SIZE_X; i++)
			{
				for (int j = 0; j < ARRAY_SIZE_Z; j++)
				{
					startTransform.setOrigin(btVector3(
						btScalar(0.2 * i),
						btScalar(2 + .2 * k),
						btScalar(0.2 * j)));

					createRigidBody(mass2, startTransform, colShape);
				}
			}
		}
	}

	void loop(float deltaTime) {
		if (dynamicsWorld) {
			dynamicsWorld->stepSimulation(deltaTime);
			dynamicsWorld->debugDrawWorld();
		}
	}

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
		dynamicsWorld->addRigidBody(body);
		return body;
	}

	~PhysiqueBullet() {
		if (dynamicsWorld)
		{
			int i;
			for (i = dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
			{
				dynamicsWorld->removeConstraint(dynamicsWorld->getConstraint(i));
			}
			for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
			{
				btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
				btRigidBody* body = btRigidBody::upcast(obj);
				if (body && body->getMotionState())
				{
					delete body->getMotionState();
				}
				dynamicsWorld->removeCollisionObject(obj);
				delete obj;
			}
		}
		//delete collision shapes
		for (int j = 0; j < collisionShapes.size(); j++)
		{
			btCollisionShape* shape = collisionShapes[j];
			delete shape;
		}
		collisionShapes.clear();

		delete dynamicsWorld;
		delete solver;
		delete overlappingPairCache;
		delete dispatcher;
		delete collisionConfig;
	}


	void addToPhysique() {

	}

};*/
#endif
