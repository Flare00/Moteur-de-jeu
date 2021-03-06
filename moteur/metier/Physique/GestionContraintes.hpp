#ifndef __GESTION_CONTRAINTES_HPP__
#define __GESTION_CONTRAINTES_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

#include <BulletDynamics/ConstraintSolver/btHingeConstraint.h>
#include <BulletDynamics/ConstraintSolver/btSliderConstraint.h>
#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>
#include <BulletDynamics/ConstraintSolver/btGearConstraint.h>
#include <BulletDynamics/ConstraintSolver/btFixedConstraint.h>
#include <BulletDynamics/ConstraintSolver/btUniversalConstraint.h>

#include <Transformation/BulletTransformation.hpp>

class GestionContraintes
{
private:
	btDiscreteDynamicsWorld *world;
	std::vector<btTypedConstraint *> contraintes;

public:
	GestionContraintes(btDiscreteDynamicsWorld *world)
	{
		this->world = world;
	}

	~GestionContraintes()
	{
		for (size_t i = 0, max = contraintes.size(); i < max; i++)
		{
			delete contraintes[i];
		}
	}

	// HINGE2
	btHingeConstraint *addHingeContrainte(BulletTransformation *rbA, BulletTransformation *rbB, glm::vec3 pivotA, glm::vec3 pivotB, glm::vec3 axisA, glm::vec3 axisB)
	{
		return addHingeContrainte(rbA->getBulletRigidbody()->getRigidbody(), rbB->getBulletRigidbody()->getRigidbody(), pivotA, pivotB, axisA, axisB);
	}
	btHingeConstraint *addHingeContrainte(btRigidBody *rbA, btRigidBody *rbB, glm::vec3 pivotA, glm::vec3 pivotB, glm::vec3 axisA, glm::vec3 axisB)
	{
		btVector3 pivotABt = btVector3(pivotA.x, pivotA.y, pivotA.z);
		btVector3 pivotBBt = btVector3(pivotB.x, pivotB.y, pivotB.z);
		btVector3 axisABt = btVector3(axisA.x, axisA.y, axisA.z);
		btVector3 axisBBt = btVector3(axisB.x, axisB.y, axisB.z);

		btHingeConstraint *c = new btHingeConstraint(*rbA, *rbB, (const btVector3)pivotABt, (const btVector3)pivotBBt, (const btVector3)axisABt, (const btVector3)axisBBt);
		this->world->addConstraint(c, true);
		this->contraintes = contraintes;
		return c;
	}

	// SLIDER
	btSliderConstraint *addSlideContrainte(BulletTransformation *rbA, BulletTransformation *rbB, btTransform frameInA = btTransform::getIdentity(), btTransform frameInB = btTransform::getIdentity(), bool useLinearReferenceFrameA = true)
	{
		return addSlideContrainte(rbA->getBulletRigidbody()->getRigidbody(), rbB->getBulletRigidbody()->getRigidbody(), frameInA, frameInB, useLinearReferenceFrameA);
	}
	btSliderConstraint *addSlideContrainte(btRigidBody *rbA, btRigidBody *rbB, btTransform frameInA = btTransform::getIdentity(), btTransform frameInB = btTransform::getIdentity(), bool useLinearReferenceFrameA = true)
	{
		btSliderConstraint *c = new btSliderConstraint(*rbA, *rbB, frameInA, frameInB, useLinearReferenceFrameA);
		this->world->addConstraint(c, true);
		this->contraintes = contraintes;
		return c;
	}

	// POINT TO POINT
	btPoint2PointConstraint *addPointToPointContrainte(BulletTransformation *rbA, BulletTransformation *rbB, glm::vec3 pointInA, glm::vec3 pointInB)
	{
		return addPointToPointContrainte(rbA->getBulletRigidbody()->getRigidbody(), rbB->getBulletRigidbody()->getRigidbody(), pointInA, pointInB);
	}

	btPoint2PointConstraint *addPointToPointContrainte(btRigidBody *rbA, btRigidBody *rbB, glm::vec3 pointInA, glm::vec3 pointInB)
	{
		btPoint2PointConstraint *c = new btPoint2PointConstraint(*rbA, *rbB, (const btVector3)btVector3(pointInA.x, pointInA.y, pointInA.z), btVector3(pointInB.x, pointInB.y, pointInB.z));
		this->world->addConstraint(c, true);
		this->contraintes = contraintes;
		return c;
	}

	// GENERIC 6 DOF
	btGeneric6DofConstraint *addGeneric6DOFContrainte(BulletTransformation *rbA, BulletTransformation *rbB, glm::vec3 originA = glm::vec3(0.0f), glm::vec3 originB = glm::vec3(0.0f), glm::vec3 rotationA = glm::vec3(0.0f), glm::vec3 rotationB = glm::vec3(0.0f), bool useLinearReferenceFrameA = true)
	{
		return addGeneric6DOFContrainte(rbA->getBulletRigidbody()->getRigidbody(), rbB->getBulletRigidbody()->getRigidbody(), originA, originB, rotationA, rotationB, useLinearReferenceFrameA);
	}

	btGeneric6DofConstraint *addGeneric6DOFContrainte(btRigidBody *rbA, btRigidBody *rbB, glm::vec3 originA = glm::vec3(0.0f), glm::vec3 originB = glm::vec3(0.0f), glm::vec3 rotationA = glm::vec3(0.0f), glm::vec3 rotationB = glm::vec3(0.0f), bool useLinearReferenceFrameA = true)
	{
		btTransform frameInA = btTransform::getIdentity(), frameInB = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(originA.x, originA.y, originA.z));
		frameInB.setOrigin(btVector3(originB.x, originB.y, originB.z));

		btQuaternion quatA, quatB;
		quatA.setEulerZYX(rotationA.z, rotationA.y, rotationA.x);
		quatB.setEulerZYX(rotationB.z, rotationB.y, rotationB.x);
		frameInA.setRotation(quatA);
		frameInB.setRotation(quatB);

		btGeneric6DofConstraint *c = new btGeneric6DofConstraint(*rbA, *rbB, frameInA, frameInB, useLinearReferenceFrameA);
		this->world->addConstraint(c, true);
		this->contraintes = contraintes;
		return c;
	}

	// GEAR
	btGearConstraint *addGearContrainte(BulletTransformation *rbA, BulletTransformation *rbB, glm::vec3 axisInA, glm::vec3 axisInB, btScalar ratio = (1.0f))
	{
		return addGearContrainte(rbA->getBulletRigidbody()->getRigidbody(), rbB->getBulletRigidbody()->getRigidbody(), axisInA, axisInB, ratio);
	}

	btGearConstraint *addGearContrainte(btRigidBody *rbA, btRigidBody *rbB, glm::vec3 axisInA, glm::vec3 axisInB, btScalar ratio = btScalar(1.0f))
	{
		btGearConstraint *c = new btGearConstraint(*rbA, *rbB, (const btVector3)btVector3(axisInA.x, axisInA.y, axisInA.z), (const btVector3)btVector3(axisInB.x, axisInB.y, axisInB.z), ratio);
		this->world->addConstraint(c, true);
		this->contraintes = contraintes;
		return c;
	}

	// Fixed
	btFixedConstraint *addFixedContrainte(BulletTransformation *rbA, BulletTransformation *rbB, glm::vec3 originA = glm::vec3(0.0f), glm::vec3 originB = glm::vec3(0.0f), glm::vec3 rotationA = glm::vec3(0.0f), glm::vec3 rotationB = glm::vec3(0.0f))
	{
		return addFixedContrainte(rbA->getBulletRigidbody()->getRigidbody(), rbB->getBulletRigidbody()->getRigidbody(), originA, originB, rotationA, rotationB);
	}

	btFixedConstraint *addFixedContrainte(btRigidBody *rbA, btRigidBody *rbB, glm::vec3 originA = glm::vec3(0.0f), glm::vec3 originB = glm::vec3(0.0f), glm::vec3 rotationA = glm::vec3(0.0f), glm::vec3 rotationB = glm::vec3(0.0f))
	{

		btTransform frameInA = btTransform::getIdentity(), frameInB = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(originA.x, originA.y, originA.z));
		frameInB.setOrigin(btVector3(originB.x, originB.y, originB.z));

		btQuaternion quatA, quatB;
		quatA.setEulerZYX(rotationA.z, rotationA.y, rotationA.x);
		quatB.setEulerZYX(rotationB.z, rotationB.y, rotationB.x);
		frameInA.setRotation(quatA);
		frameInB.setRotation(quatB);

		btFixedConstraint *c = new btFixedConstraint(*rbA, *rbB, frameInA, frameInB);
		this->world->addConstraint(c, true);
		this->contraintes = contraintes;
		return c;
	}

	// Universal
	btUniversalConstraint *addUniversalContrainte(BulletTransformation *rbA, BulletTransformation *rbB, glm::vec3 anchor, glm::vec3 axis1, glm::vec3 axis2)
	{
		return addUniversalContrainte(rbA->getBulletRigidbody()->getRigidbody(), rbB->getBulletRigidbody()->getRigidbody(), anchor, axis1, axis2);
	}

	btUniversalConstraint *addUniversalContrainte(btRigidBody *rbA, btRigidBody *rbB, glm::vec3 anchor, glm::vec3 axis1, glm::vec3 axis2)
	{
		btUniversalConstraint *c = new btUniversalConstraint(*rbA, *rbB, (const btVector3)btVector3(anchor.x, anchor.y, anchor.z), (const btVector3)btVector3(axis1.x, axis1.y, axis1.z), (const btVector3)btVector3(axis2.x, axis2.y, axis2.z));
		this->world->addConstraint(c, true);
		this->contraintes = contraintes;
		return c;
	}
};

#endif // !__GESTION_CONTRAINTES_HPP__
