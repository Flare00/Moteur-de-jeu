#ifndef __CANON_HPP__
#define __CANON_HPP__

#include <glm/glm.hpp>
#include <GameObject/ModeleLOD.hpp>
#include <Physique/BulletRigidbody.hpp>

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
        go = new GameObject("Canon");

        // PHYSIC
        cylinder_Rigid = BulletRigidbody::generateCylinder(glm::vec3(0.37f, 1.86f, 0.37f), 0.0f);
        base_Rigid = BulletRigidbody::generateAABB(glm::vec3(2.21f, 0.46f, 0.2f), 0.0f);
        bind_Rigid_left = BulletRigidbody::generateCylinder(glm::vec3(0.1f, 0.2f, 0.1f), 0.0f);
        wheel_Rigid_left = BulletRigidbody::generateCylinder(glm::vec3(1.0f, 0.1f, 1.0f), 0.0f);

        // MODELES
        cylinder = new ModeleLOD("canon", globalShader, ModeleComponent::OBJ, "Model/Canon/canon.obj", cylinder_Rigid);
        base = new ModeleLOD("base", globalShader, ModeleComponent::OBJ, "Model/Canon/base.obj", base_Rigid);
        bind_left = new ModeleLOD("bind", globalShader, ModeleComponent::OBJ, "Model/Canon/bind.obj", bind_Rigid_left);
        bind_right = bind_left->duplicate();
        wheel_left = new ModeleLOD("wheel", globalShader, ModeleComponent::OBJ, "Model/Canon/wheel.obj", wheel_Rigid_left);
        wheel_right = wheel_left->duplicate();

        // DUPLICATE
        bind_Rigid_right = bind_right->getRigidBody();
        wheel_Rigid_right = wheel_right->getRigidBody();

        go->addChild(cylinder);
        go->addChild(base);
        go->addChild(bind_left);
        go->addChild(bind_right);
        go->addChild(wheel_left);
        go->addChild(wheel_right);

        // MOVE
        cylinder->getTransform()->setTranslate(glm::vec3(0, 1, 0));
        bind_left->getTransform()->setTranslate(glm::vec3(0, 0, -1));
        bind_right->getTransform()->setTranslate(glm::vec3(0, 0, 1));
        wheel_left->getTransform()->setTranslate(glm::vec3(0, 0, -2));
        wheel_right->getTransform()->setTranslate(glm::vec3(0, 0, 2));
    }

    GameObject *getGameObject()
    {
        return this->go;
    }
};

#endif