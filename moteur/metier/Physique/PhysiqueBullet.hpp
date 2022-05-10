#ifndef __PHYSIQUE_BULLET_HPP__
#define __PHYSIQUE_BULLET_HPP__

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Physique/DebugDrawer.hpp>
#include "GestionContraintes.hpp"
#include <Physique/CollisionFilter.hpp>
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Z 5

class PhysiqueBullet
{

public:
	bool debugState = false;

	GestionContraintes *gestionContraintes = NULL;
	CollisionFilter *collisionFilter = NULL;
	btDiscreteDynamicsWorld *dynamicsWorld = NULL;;
	btSequentialImpulseConstraintSolver *solver = NULL;
	btDbvtBroadphase *overlappingPairCache = NULL;
	btCollisionDispatcher *dispatcher = NULL;
	btDefaultCollisionConfiguration *collisionConfig = NULL;

	std::vector<BulletRigidbody *> rigidbodies;

	PhysiqueBullet()
	{
	}

	void init(DebugDrawer * debug = NULL)
	{
		// Mise en place de la configuration pour collision
		collisionConfig = new btDefaultCollisionConfiguration();
		// Mono Thread dispatcher (Regarder les BulletMultiThreaded pour le multi Thread)
		dispatcher = new btCollisionDispatcher(collisionConfig);

		collisionFilter = new CollisionFilter();

		// Overlapping broadphase , on peut essayer btAxis3Sweep
		overlappingPairCache = new btDbvtBroadphase();
		// Constraint solver (peut �tre multi thread, m�me chose que le dispatcher)
		solver = new btSequentialImpulseConstraintSolver();

		// Genere le monde dynamique
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
		// Ajoute la gravit�
		dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

		if (debug != NULL)
		{
			dynamicsWorld->setDebugDrawer(debug);
		}

		dynamicsWorld->getPairCache()->setOverlapFilterCallback(collisionFilter);

		gestionContraintes = new GestionContraintes(this->dynamicsWorld);
	}

	void loop(float deltaTime)
	{
		if (dynamicsWorld != NULL)
		{
			dynamicsWorld->stepSimulation(deltaTime);
			if (dynamicsWorld != NULL && debugState)
			{
				dynamicsWorld->debugDrawWorld();
			}
		}
	}

	void draw() {


	}

	void toogleDebug() {
		debugState = !debugState;
	}

	GestionContraintes *getGestionContraintes()
	{
		return this->gestionContraintes;
	}

	void addGhostObjectToPhysique(btGhostObject *ghost)
	{
		this->dynamicsWorld->addCollisionObject(ghost);
	}

	void removeGhostObjectToPhysique(btGhostObject *ghost)
	{
		this->dynamicsWorld->removeCollisionObject(ghost);
	}

	void addRigidbodyToPhysique(BulletRigidbody *rigidbody, int group = 1, int mask = 1)
	{
		this->dynamicsWorld->addRigidBody(rigidbody->getRigidbody(), group, mask);
		this->rigidbodies.push_back(rigidbody);
	}

	void removeRigidbodyFromPhysique(BulletRigidbody *rigidbody)
	{
		this->dynamicsWorld->removeRigidBody(rigidbody->getRigidbody());
		bool found = false;
		size_t i = 0;
		for (size_t max = this->rigidbodies.size(); i < max && !found; i++)
		{
			if (this->rigidbodies[i]->getRigidbody() == rigidbody->getRigidbody())
			{
				found = true;
			}
		}
		if (found)
		{
			this->rigidbodies.erase(this->rigidbodies.begin() + i);
		}
	}

	~PhysiqueBullet()
	{
		if (dynamicsWorld)
		{
			int i;
			for (i = dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
			{
				dynamicsWorld->removeConstraint(dynamicsWorld->getConstraint(i));
			}
			for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
			{
				btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[i];
				btRigidBody *body = btRigidBody::upcast(obj);
				if (body && body->getMotionState())
				{
					delete body->getMotionState();
				}
				dynamicsWorld->removeCollisionObject(obj);
				delete obj;
			}
		}

		delete dynamicsWorld;
		dynamicsWorld = NULL;
		delete solver;
		solver = NULL;

		delete overlappingPairCache;
		overlappingPairCache = NULL;

		delete dispatcher;
		dispatcher = NULL;

		delete collisionConfig;
		collisionConfig = NULL;

	}
};
#endif
