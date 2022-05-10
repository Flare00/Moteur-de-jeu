#ifndef __CANON_HPP__
#define __CANON_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/perpendicular.hpp>

#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>

#include <GameObject/ModeleLOD.hpp>
#include <Physique/BulletRigidbody.hpp>
#include <Physique/PhysiqueBullet.hpp>
#include <Transformation/ITransformation.hpp>
#include <Texture.hpp>

class Canon
{
protected:
	// Physic
	BulletRigidbody* cylinder_Rigid;
	BulletRigidbody* base_Rigid;
	BulletRigidbody* bind_Rigid_left;
	BulletRigidbody* bind_Rigid_right;
	BulletRigidbody* wheel_Rigid_left;
	BulletRigidbody* wheel_Rigid_right;
	BulletRigidbody* contre_poid_rigid;

	GameObject* go;
	BulletRigidbody* rigid;

	// Modele
	ModeleLOD* cylinder;
	ModeleLOD* base;
	ModeleLOD* bind_left;
	ModeleLOD* bind_right;
	ModeleLOD* wheel_left;
	ModeleLOD* wheel_right;

	int group, mask;

public:
	Canon(GlobalShader* globalShader)
	{
		rigid = BulletRigidbody::generateAABB(glm::vec3(0.1f), 1.0f);
		go = new GameObject("Canon", NULL, rigid);

		Texture* texture = new Texture("Textures/texture_test.jpg");

		// PHYSIC
		cylinder_Rigid = BulletRigidbody::generateCylinder(glm::vec3(0.37f, 1.86f, 0.37f), 20.0f, ITransformation::generateMatrix(glm::vec3(-0.4f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, M_PI / 2.0f)));
		base_Rigid = BulletRigidbody::generateAABB(glm::vec3(2.21f, 0.46f, 0.2f), 50.0f, ITransformation::generateMatrix(glm::vec3(-0.35f, 0.0f, 0.0f)));
		bind_Rigid_left = BulletRigidbody::generateCylinder(glm::vec3(0.1f, 0.2f, 0.1f), 5.0f, ITransformation::generateMatrix(glm::vec3(0.0f, 0.0f, 0.20f), glm::vec3(M_PI / 2.0f, 0.0f, 0.0f)));
		wheel_Rigid_left = BulletRigidbody::generateCylinder(glm::vec3(1.0f, 0.1f, 1.0f), 5.0f, ITransformation::generateMatrix(glm::vec3(0.0f), glm::vec3(M_PI / 2.0f, 0.0f, 0.0f)));
		contre_poid_rigid = BulletRigidbody::generateAABB(glm::vec3(0.1f, 0.1f, 0.1f), 20.0f);
		// MODELES
		cylinder = new ModeleLOD("canon", globalShader, ModeleComponent::OBJ, "Model/Canon/canon.obj", cylinder_Rigid, NULL, texture);
		base = new ModeleLOD("base", globalShader, ModeleComponent::OBJ, "Model/Canon/base.obj", base_Rigid, NULL, texture);
		bind_left = new ModeleLOD("bind", globalShader, ModeleComponent::OBJ, "Model/Canon/bind.obj", bind_Rigid_left, NULL, texture);
		bind_right = bind_left->duplicate();
		wheel_left = new ModeleLOD("wheel", globalShader, ModeleComponent::OBJ, "Model/Canon/wheel.obj", wheel_Rigid_left, NULL, texture);
		wheel_right = wheel_left->duplicate();

		// DUPLICATE
		bind_Rigid_right = bind_right->getRigidBody();
		wheel_Rigid_right = wheel_right->getRigidBody();

		// ADD TO GAMEOBJECT
		go->addChild(cylinder);
		go->addChild(base);
		go->addChild(bind_left);
		go->addChild(bind_right);
		go->addChild(wheel_left);
		go->addChild(wheel_right);

		glm::vec3 pos(0, -6.5f, 0);

		cylinder->getTransform()->setTranslate(pos);
		base->getTransform()->setTranslate(pos);
		bind_left->getTransform()->setTranslate(pos);
		bind_right->getTransform()->setTranslate(pos);
		wheel_left->getTransform()->setTranslate(pos);
		wheel_right->getTransform()->setTranslate(pos);
		
	}

	GameObject* getGameObject()
	{
		return this->go;
	}

	BulletTransformation* getTransform()
	{
		return this->go->getBulletTransform();
	}

	void addToPhysique(PhysiqueBullet* physique, int group = 2, int mask = 0)
	{
		this->group = group;
		this->mask = mask;
		physique->addRigidbodyToPhysique(this->cylinder_Rigid, group, mask);
		physique->addRigidbodyToPhysique(this->base_Rigid, group, mask);
		physique->addRigidbodyToPhysique(this->contre_poid_rigid, group, mask);
		physique->addRigidbodyToPhysique(this->bind_Rigid_left, group, mask);
		physique->addRigidbodyToPhysique(this->bind_Rigid_right, group, mask);
		physique->addRigidbodyToPhysique(this->wheel_Rigid_left, group, mask);
		physique->addRigidbodyToPhysique(this->wheel_Rigid_right, group, mask);
		


		// CONSTRAINTS
		physique->getGestionContraintes()->addFixedContrainte(
			this->base_Rigid->getRigidbody(),
			this->cylinder_Rigid->getRigidbody(),
			glm::vec3(0.8f, 0.7f, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f, 0.0f, M_PI / 2.0f));
		physique->getGestionContraintes()->addFixedContrainte(
			this->base_Rigid->getRigidbody(),
			this->contre_poid_rigid->getRigidbody(),
			glm::vec3(-1.0f, -0.5f, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f));
		physique->getGestionContraintes()->addFixedContrainte(
			this->base_Rigid->getRigidbody(),
			this->bind_Rigid_left->getRigidbody(),
			glm::vec3(0.0f, 0.1f, -0.4f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec3(M_PI / 2.0f, 0.0f, 0.0f));
		physique->getGestionContraintes()->addFixedContrainte(
			this->base_Rigid->getRigidbody(),
			this->bind_Rigid_right->getRigidbody(),
			glm::vec3(0.f, 0.1f, 0.4f),
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			glm::vec3(-M_PI / 2.0f, 0.0f, 0.0f));
		physique->getGestionContraintes()->addHingeContrainte(
			this->bind_Rigid_left->getRigidbody(),
			this->wheel_Rigid_left->getRigidbody(),
			glm::vec3(0.0f, 0.1f, 0.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		physique->getGestionContraintes()->addHingeContrainte(
			this->bind_Rigid_right->getRigidbody(),
			this->wheel_Rigid_right->getRigidbody(),
			glm::vec3(0.0f, 0.1f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1, 0),
			glm::vec3(0.0f, 1, 0));
		this->wheel_Rigid_right->getRigidbody()->setFriction(btScalar(10.0f));
		this->wheel_Rigid_left->getRigidbody()->setFriction(btScalar(10.0f));
	}

	void removeFromPhysique(PhysiqueBullet* physique)
	{
		physique->removeRigidbodyFromPhysique(this->cylinder_Rigid);
		physique->removeRigidbodyFromPhysique(this->contre_poid_rigid);
		physique->removeRigidbodyFromPhysique(this->base_Rigid);
		physique->removeRigidbodyFromPhysique(this->bind_Rigid_left);
		physique->removeRigidbodyFromPhysique(this->bind_Rigid_right);
		physique->removeRigidbodyFromPhysique(this->wheel_Rigid_left);
		physique->removeRigidbodyFromPhysique(this->wheel_Rigid_right);
	}

	glm::vec3 getFront() {


		/*btScalar x, y, z;
		this->cylinder_Rigid->getRigidbody()->getWorldTransform().getRotation().getEulerZYX(z, y, x);
		glm::vec3 res(-z, y, x);
		return glm::normalize(res);*/

		btVector3 worldFront = this->cylinder_Rigid->getRigidbody()->getWorldTransform().getBasis() * btVector3(0.0f, 1.0f, 0.0f);
		return glm::normalize(glm::vec3(worldFront.getX(), worldFront.getY(), worldFront.getZ()));
	}

	glm::vec3 getCanonPos() {
		btVector3 vec = this->cylinder_Rigid->getRigidbody()->getWorldTransform().getOrigin();
		return (const glm::vec3)glm::vec3(vec.getX(), vec.getY(), vec.getZ());
	}

	void move(int dir)
	{
		btMatrix3x3 world = this->wheel_Rigid_left->getRigidbody()->getWorldTransform().getBasis();
		btVector3 worldTorque = world * btVector3(0.0f, 100.0f, 0.0f);
		btVector3 worldFront = world * btVector3(0.0f, 1.0f, 0.0f);
		btVector3 worldRight = world * btVector3(1.0f, 0.0f, 0.0f);
		this->wheel_Rigid_left->getRigidbody()->activate();
		this->wheel_Rigid_right->getRigidbody()->activate();
		if (dir == 0) {
			this->wheel_Rigid_left->getRigidbody()->applyTorque((const btVector3)worldTorque);
			this->wheel_Rigid_right->getRigidbody()->applyTorque((const btVector3)worldTorque);

		}
		else if (dir == 1) {
			this->wheel_Rigid_left->getRigidbody()->applyTorque((const btVector3)-worldTorque);
			this->wheel_Rigid_right->getRigidbody()->applyTorque((const btVector3)-worldTorque);

		}
		else if (dir == 2) {
			this->wheel_Rigid_left->getRigidbody()->applyTorque((const btVector3)-worldTorque *1.5f);
			this->wheel_Rigid_right->getRigidbody()->applyTorque((const btVector3)worldTorque *1.5f);

		}
		else if (dir == 3) {
			this->wheel_Rigid_left->getRigidbody()->applyTorque((const btVector3)worldTorque * 1.5f);
			this->wheel_Rigid_right->getRigidbody()->applyTorque((const btVector3)-worldTorque * 1.5f);
		}
	}

	int getGroup() {
		return group;
	}

	int getMask() {
		return mask;
	}
};

#endif