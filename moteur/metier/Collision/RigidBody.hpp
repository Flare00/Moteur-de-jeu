#ifndef __RIGID_BODY_HPP__
#define __RIGID_BODY_HPP__
#include <cmath>
#include <GameObject/GameObject.hpp>
#include <Component.hpp>
#include "CollisionComponent.hpp"
class RigidBody : public Component
{
private:
    GameObject *parent;
    CollisionComponent *collision = NULL;
    CollisionComponent *collisionModele = NULL;
    glm::vec3 vitesse;
    glm::vec3 forces;
    glm::vec3 correction;
    float masse;
    float coefRestit;
    float friction;
    bool staticObj;

public:
    RigidBody(GameObject *parent, bool staticObj = false, float m = 1.0f, glm::vec3 v = glm::vec3(0.0f), float cR = 0.5f, float f = 0.0f) : Component(Component::Type::RIGIDBODY)
    {
        this->staticObj = staticObj;

        this->parent = parent;
        if (staticObj)
        {
            this->masse = FLT_MAX;

            std::cout << "Masse : " << this->masse << std::endl;
        }
        else
        {
            this->masse = m;
            this->vitesse = v;
        }
        this->coefRestit = cR;
        this->friction = f;
        this->forces = glm::vec3(0.0f);
        this->correction = glm::vec3(0.0f);
    }
    ~RigidBody() {}

    void generateBoundingBox(std::vector<glm::vec3> vertexArray)
    {
        this->collision = new CollisionComponent(vertexArray);
    }

    void generateSphereCollision(glm::vec3 center, float radius)
    {
        this->collision = new CollisionComponent(center, radius);
    }

    void generateModeleCollision(std::vector<glm::vec3> vertexArray, std::vector<unsigned int> indices)
    {
        this->collisionModele = new CollisionComponent(vertexArray, indices);
    }

    void addForces(glm::vec3 force)
    {
        this->forces += force * (1.0f / this->masse);
    }

    void addCorrection(glm::vec3 correction)
    {
        this->correction += correction;
    }

    void setVitesse(glm::vec3 v)
    {
        this->vitesse = v;
    }

    void applyVitesse(float deltaTime, float coefAmortissement = 1.0f)
    {
        if (!staticObj)
        {
            this->vitesse += deltaTime * forces;
            this->vitesse *= coefAmortissement;
            for (int i = 0; i < 3; i++)
            {
                if (std::abs(this->vitesse[i]) < 0.001f)
                {
                    this->vitesse[i] = 0.0f;
                }
            }

            parent->getTransform()->translate(vitesse * deltaTime);
            parent->getTransform()->translate(correction * this->getInvMass());
            this->correction = glm::vec3(0.0f);
            this->forces = glm::vec3(0.0f);
        }
    }

    CollisionComponent *getCollision()
    {
        return this->collision;
    }

    CollisionComponent *getCollisionModele()
    {
        return this->collisionModele;
    }

    glm::vec3 computeVitesseRelative(RigidBody *r)
    {
        return r->getVitesse() - this->vitesse;
    }

    glm::vec3 getVitesse()
    {
        return this->vitesse;
    }

    float getMass()
    {
        return this->masse;
    }

    float getInvMass()
    {
        return 1.0f / this->masse;
    }

    float getCoefRestitution()
    {
        return this->coefRestit;
    }

    float getFriction()
    {
        return this->friction;
    }

    bool isStatic()
    {
        return this->staticObj;
    }

    bool hasModeleCollision()
    {
        return this->collisionModele != NULL;
    }
};

#endif