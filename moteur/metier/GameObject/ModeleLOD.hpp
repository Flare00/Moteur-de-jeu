#ifndef __MODELE_LOD_H__
#define __MODELE_LOD_H__

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <objloader.hpp>

#include <Light/Material.hpp>
#include <Texture.hpp>
#include <Collision/CollisionComponent.hpp>
#include <Shader/GlobalShader.hpp>
#include <Tools/SimplifyMesh.hpp>

#include "GameObject.hpp"
#include <GameObject/ModeleComponent.hpp>

class ModeleLOD : public GameObject
{
protected:
	// 0 : High Poly, 1 : Low Poly, 2 : Impostor
	ModeleComponent *modeles[3];
	float distanceLOD[2];
	RigidBody* rigidbody= NULL;
public:
	// --- CONSTRUCTEURS ET DESTRUCTEURS ---
	ModeleLOD(std::string id, ModeleComponent * high, ModeleComponent * low = NULL, ModeleComponent * imposteur = NULL, GameObject *parent = NULL) : GameObject(id, parent)
	{
		modeles[0] = high;
		modeles[1] = low;
		modeles[2] = imposteur;
		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 100.0f;

		this->addComponent(modeles[0]);
		if (modeles[1] != NULL) 
			this->addComponent(modeles[1]);
		if(modeles[2] != NULL)
			this->addComponent(modeles[2]);
	}

	ModeleLOD(std::string id, GlobalShader *shader, std::string fileOff, GameObject *parent = NULL) : GameObject(id, parent)
	{
		modeles[0] = new ModeleComponent(shader, fileOff);
		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 100.0f;
		this->addComponent(modeles[0]);


	}

	ModeleLOD(std::string id, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> normals, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, GlobalShader *shader, GameObject *parent = NULL) : GameObject(id, parent)
	{
		modeles[0] = new ModeleComponent(shader, indexed_vertices, normals, indices, texCoords);
		this->addComponent(modeles[0]);

		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 25.0f;
	}

	virtual ~ModeleLOD()
	{
		for (int i = 0; i < 3; i++)
		{
			delete modeles[i];
		}
	}

	// --- METHODES ---
	virtual void compute(Camera *camera, bool dfs = true)
	{
		float distance = -1.0f;
		if (this->getRigidBody() != NULL) {
			this->getRigidBody()->getCollision()->apply(this->getTransformMatrix());
			std::vector<glm::vec3> boxCoords = this->getRigidBody()->getCollision()->getCoords();
			for (size_t i = 0, max = boxCoords.size(); i < max && distance < 0.0f; i++)
			{
				distance = camera->distanceFromCamera(boxCoords[i]);
			}

			if (distance >= 0.0f)
			{
				int level = 2;
				if (distance <= this->distanceLOD[0])
				{
					level = 0;
				}
				else if (distance <= this->distanceLOD[1])
				{
					level = 1;
				}
				draw(camera, level);
			}
			else {
				draw(camera, 0);
			}
		}
		else {
			draw(camera, 0);
		}

		if (dfs)
			GameObject::compute(camera, dfs);
	}

	void draw(Camera* camera, int level)
	{
		while (level >= 0 && modeles[level] == NULL) {
			level--;
		}
		if (level >= 0) {
			modeles[level]->draw(camera, this->getTransformMatrix(), this->rigidbody);
		}
	}

	// ---- GETTER ET SETTER ---

	ModeleComponent *getModele(int level)
	{
		return this->modeles[level];
	}

	void setRigidBody(RigidBody* body) {
		if(this->rigidbody != NULL)
		{
			delete this->rigidbody;
		}

		this->rigidbody = body;
		if (this->modeles[0]->getIndexedVertices().size() > 0) 
		{
			this->rigidbody->generateBoundingBox(this->modeles[0]->getIndexedVertices());
		}

		this->addComponent(this->rigidbody);
	}
	void setRigidBody(RigidBody* body, glm::vec3 center, float radius) {
		if (this->rigidbody != NULL)
		{
			delete this->rigidbody;
		}

		this->rigidbody = body;
		this->rigidbody->generateSphereCollision(center, radius);

		this->addComponent(this->rigidbody);
	}
	RigidBody* getRigidBody() {
		return this->rigidbody;
	}

	void setCollisionState(bool coll) {
		this->rigidbody->getCollision()->setActif(coll);
	}
};

#endif