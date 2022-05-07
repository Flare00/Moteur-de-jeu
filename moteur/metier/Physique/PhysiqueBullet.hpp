#ifndef __PHYSIQUE_BULLET_HPP__
#define __PHYSIQUE_BULLET_HPP__

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Physique/DebugDrawer.hpp>
#include "GestionContraintes.hpp"
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Z 5


class PhysiqueBullet {
	
public:
	GestionContraintes* gestionContraintes = NULL;
	btGhostObject* ghost = NULL;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btSequentialImpulseConstraintSolver* solver;
	btDbvtBroadphase* overlappingPairCache;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfig;

	btOverlappingPairCallback* ghostCallback = NULL;

	btBoxShape* box;
	btTransform groundTransform;
	btRigidBody* boxBody;

	std::vector<BulletRigidbody*> rigidbodies;
	PhysiqueBullet() {
	}


	void init(DebugDrawer* debug = NULL) {
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
		dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
		if (debug != NULL) {
			dynamicsWorld->setDebugDrawer(debug);
		}

		ghostCallback = new btGhostPairCallback();
		dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(ghostCallback);

	}

	void loop(float deltaTime) {
		if (dynamicsWorld) {
			dynamicsWorld->stepSimulation(deltaTime);
			dynamicsWorld->debugDrawWorld();
		}
	}


	GestionContraintes* getGestionContraintes() {
		return this->gestionContraintes;
	}

	void addGhostObjectToPhysique(btGhostObject* ghost) {
		this->dynamicsWorld->addCollisionObject(ghost);
		this->ghost = ghost;
	}

	void removeGhostObjectToPhysique(btGhostObject* ghost) {
		this->dynamicsWorld->removeCollisionObject(ghost);
	}

	void addRigidbodyToPhysique(BulletRigidbody * rigidbody) {
		this->dynamicsWorld->addRigidBody(rigidbody->getRigidbody());
		this->rigidbodies.push_back(rigidbody);
	}

	void removeRigidbodyFromPhysique(BulletRigidbody* rigidbody) {
		this->dynamicsWorld->removeRigidBody(rigidbody->getRigidbody());
		bool found = false;
		size_t i = 0;
		for (size_t max = this->rigidbodies.size(); i < max && !found; i++) {
			if(this->rigidbodies[i]->getRigidbody() == rigidbody->getRigidbody()){
				found = true;
			}
		}
		if (found) {
			this->rigidbodies.erase(this->rigidbodies.begin() + i);
		}
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
		/*for (int j = 0; j < collisionShapes.size(); j++)
		{
			btCollisionShape* shape = collisionShapes[j];
			delete shape;
		}
		collisionShapes.clear();*/

		delete dynamicsWorld;
		delete solver;
		delete overlappingPairCache;
		delete dispatcher;
		delete collisionConfig;
	}

	/*~PhysiqueBullet() {
		size_t last;
		while ((last = rigidbodies.size()) > 0) {
			BulletRigidbody* b = rigidbodies[last - 1];
			rigidbodies.pop_back();
			this->dynamicsWorld->removeRigidBody(b->getRigidbody());
			delete b;
		}
	}*/


};
#endif
