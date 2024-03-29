﻿#ifndef __MODELE_LOD_H__
#define __MODELE_LOD_H__

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <Light/Material.hpp>
#include <Texture.hpp>
#include <Shader/GlobalShader.hpp>

#include "GameObject.hpp"
#include <GameObject/ModeleComponent.hpp>

class ModeleLOD : public GameObject
{
protected:
	// 0 : High Poly, 1 : Low Poly, 2 : Impostor
	ModeleComponent *modeles[3];
	float distanceLOD[2];

public:
	// --- CONSTRUCTEURS ET DESTRUCTEURS ---
	ModeleLOD(std::string id, ModeleComponent *high, ModeleComponent *low = NULL, ModeleComponent *imposteur = NULL, BulletRigidbody *rigidBody = NULL, GameObject *parent = NULL) : GameObject(id, parent, rigidBody)
	{
		modeles[0] = high;
		modeles[1] = low;
		modeles[2] = imposteur;
		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 100.0f;

		this->addComponent(modeles[0]);
		if (modeles[1] != NULL)
			this->addComponent(modeles[1]);
		if (modeles[2] != NULL)
			this->addComponent(modeles[2]);
	}

	ModeleLOD(std::string id, GlobalShader *shader, ModeleComponent::FileType type, std::string file, BulletRigidbody *rigidBody = NULL, GameObject *parent = NULL, Texture *texture = NULL) : GameObject(id, parent, rigidBody)
	{
		modeles[0] = new ModeleComponent(shader, type, file);
		modeles[0]->addTexture(texture, false);
		modeles[1] = NULL;
		modeles[2] = NULL;
		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 100.0f;
		this->addComponent(modeles[0]);
	}

	ModeleLOD(std::string id, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> normals, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, GlobalShader *shader, BulletRigidbody *rigidBody = NULL, GameObject *parent = NULL) : GameObject(id, parent, rigidBody)
	{
		modeles[0] = new ModeleComponent(shader, indexed_vertices, normals, indices, texCoords);
		modeles[1] = NULL;
		modeles[2] = NULL;
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
	virtual void compute(CameraData *data, Lightning *lights, bool dfs = true)
	{
		float distance = -1.0f;
		distance = data->distanceFrom(this->getPosition());
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
			draw(data, lights, level);
		}
		else
		{
			draw(data, lights, 0);
		}

		if (dfs)
			GameObject::compute(data, lights, dfs);
	}

	void draw(CameraData *data, Lightning *lights, int level)
	{
		while (level >= 0 && modeles[level] == NULL)
		{
			level--;
		}
		if (level >= 0)
		{
			modeles[level]->draw(data, lights, this->getTransformMatrix(), this->getPosition());
		}
	}

	ModeleLOD *duplicate()
	{
		return new ModeleLOD(this->identifier, this->modeles[0], this->modeles[1], this->modeles[2], (this->rigidBody != NULL ? this->rigidBody->duplicate() : NULL), this->parent);
	}

	// ---- GETTER ET SETTER ---

	ModeleComponent *getModele(int level)
	{
		return this->modeles[level];
	}
};

#endif