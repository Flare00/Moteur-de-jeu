#ifndef __RIGID_BODY_HPP__
#define __RIGID_BODY_HPP__
#include <cmath>
#include <GameObject/GameObject.hpp>
#include <Component.hpp>
#include "Collision.hpp"
class RigidBody : public Component
{
private:
    GameObject *parent;
    Collision *collision = NULL;
    glm::vec3 vitesse;
    glm::vec3 forces;
    float masse;
    float coefRestit;
    float friction;
    bool isStatic;

public:
    RigidBody(GameObject *parent, bool isStatic = false, float m = 1.0f, glm::vec3 v = glm::vec3(0.0f), float cR = 0.5f, float f = 0.0f) : Component(Component::Type::RIGIDBODY)
    {
        this->parent = parent;
        this->masse = m;
        this->vitesse = v;
        this->coefRestit = cR;
        this->friction = f;
        this->forces = glm::vec3(0.0f);
        this->isStatic = false;
    }
    ~RigidBody() {}

    void generateCollision(std::vector<glm::vec3> vertexArray)
    {
        this->collision = new Collision(vertexArray);
    }

    void addForces(glm::vec3 force)
    {
        this->forces += force * (1.0f / this->masse);
    }

    void setVitesse(glm::vec3 v)
    {
        this->vitesse = v;
    }

    void applyVitesse(float deltaTime, float coefAmortissement = 1.0f)
    {
        if (!isStatic)
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
            this->forces = glm::vec3(0.0f);
        }
    }

    Collision *getCollision()
    {
        return this->collision;
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
};

#endif