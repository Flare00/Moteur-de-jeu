#ifndef __CANON_HPP__
#define __CANON_HPP__

#include <glm/glm.hpp>
#include <GameObject/ModeleLOD.hpp>
#include <Physique/BulletRigidbody.hpp>
#include <Physique/PhysiqueBullet.hpp>
#include <Transformation/ITransformation.hpp>
#include <Texture.hpp>

class Canon
{
protected:
    // Physic
    BulletRigidbody *cylinder_Rigid;
    BulletRigidbody *base_Rigid;
    BulletRigidbody *bind_Rigid_left;
    BulletRigidbody *bind_Rigid_right;
    BulletRigidbody *wheel_Rigid_left;
    BulletRigidbody *wheel_Rigid_right;

    GameObject *go;
    BulletRigidbody *rigid;

    // Modele
    ModeleLOD *cylinder;
    ModeleLOD *base;
    ModeleLOD *bind_left;
    ModeleLOD *bind_right;
    ModeleLOD *wheel_left;
    ModeleLOD *wheel_right;

public:
    Canon(GlobalShader *globalShader)
    {
        rigid = new BulletRigidbody();
        go = new GameObject("Canon", NULL, rigid);

        Texture *texture = new Texture("Textures/texture_test.jpg");

        // PHYSIC
        cylinder_Rigid = BulletRigidbody::generateCylinder(glm::vec3(0.37f, 1.86f, 0.37f), 1.0f, ITransformation::generateMatrix(glm::vec3(-0.4f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, M_PI / 2.0f)));
        base_Rigid = BulletRigidbody::generateAABB(glm::vec3(2.21f, 0.46f, 0.2f), 1.0f, ITransformation::generateMatrix(glm::vec3(-0.35f, 0.0f, 0.0f)));
        bind_Rigid_left = BulletRigidbody::generateCylinder(glm::vec3(0.1f, 0.2f, 0.1f), 1.0f, ITransformation::generateMatrix(glm::vec3(0.0f, 0.0f, 0.20f), glm::vec3(M_PI / 2.0f, 0.0f, 0.0f)));
        wheel_Rigid_left = BulletRigidbody::generateCylinder(glm::vec3(1.0f, 0.1f, 1.0f), 1.0f, ITransformation::generateMatrix(glm::vec3(0.0f), glm::vec3(M_PI / 2.0f, 0.0f, 0.0f)));

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

        cylinder->getTransform()->setTranslate(glm::vec3(0.0f, -9.0f, 0.0f));
        base->getTransform()->setTranslate(glm::vec3(0.0f, -9.0f, 0.0f));
        bind_left->getTransform()->setTranslate(glm::vec3(0.0f, -9.0f, 0.0f));
        bind_right->getTransform()->setTranslate(glm::vec3(0.0f, -9.0f, 0.0f));
        wheel_left->getTransform()->setTranslate(glm::vec3(0.0f, -9.0f, 0.0f));
        wheel_right->getTransform()->setTranslate(glm::vec3(0.0f, -9.0f, 0.0f));
    }

    GameObject *getGameObject()
    {
        return this->go;
    }

    BulletTransformation *getTransform()
    {
        return this->go->getBulletTransform();
    }

    void addToPhysique(PhysiqueBullet *physique, int group = 2, int mask = 1)
    {
        physique->addRigidbodyToPhysique(this->rigid, 0, 0);
        physique->addRigidbodyToPhysique(this->cylinder_Rigid, group, mask);
        physique->addRigidbodyToPhysique(this->base_Rigid, group, mask);
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
            this->bind_Rigid_left->getRigidbody(),
            glm::vec3(0.0f, 0.1f, 0.4f),
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(M_PI / 2.0f, 0.0f, 0.0f));
        physique->getGestionContraintes()->addFixedContrainte(
            this->base_Rigid->getRigidbody(),
            this->bind_Rigid_right->getRigidbody(),
            glm::vec3(0.f, 0.1f, -0.4f),
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(-M_PI / 2.0f, 0.0f, 0.0f));
        physique->getGestionContraintes()->addFixedContrainte(
            this->bind_Rigid_left->getRigidbody(),
            this->wheel_Rigid_left->getRigidbody(),
            glm::vec3(0.0f, -0.1f, 0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f));
        physique->getGestionContraintes()->addFixedContrainte(
            this->bind_Rigid_right->getRigidbody(),
            this->wheel_Rigid_right->getRigidbody(),
            glm::vec3(0.f, -0.1f, 0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f));
    }

    void removeFromPhysique(PhysiqueBullet *physique)
    {
        physique->removeRigidbodyFromPhysique(this->cylinder_Rigid);
        physique->removeRigidbodyFromPhysique(this->base_Rigid);
        physique->removeRigidbodyFromPhysique(this->bind_Rigid_left);
        physique->removeRigidbodyFromPhysique(this->bind_Rigid_right);
        physique->removeRigidbodyFromPhysique(this->wheel_Rigid_left);
        physique->removeRigidbodyFromPhysique(this->wheel_Rigid_right);
    }
};

#endif