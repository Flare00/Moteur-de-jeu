#ifndef __H_TERRAIN__
#define __H_TERRAIN__

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
#include <Tools/PrimitiveMesh.hpp>

#include "GameObject.hpp"
#include <GameObject/ModeleHeightComponent.hpp>

class Terrain : public GameObject
{
private:
	GlobalShaderExtended *shader;
	Material material;
	float width, height; // Taille
	int resX, resY;		 // Résolution
	int multiplicatorCollision = 4;
	float zHeight;

protected:
	ModeleHeightComponent *modele;
	RigidBody* rigidbody;
	// RIGIDBODY (collision)


public:
	Terrain(std::string id, GlobalShaderExtended *shader, Texture *texture, Texture *heightMap, float zHeight = 1.0f, float width =32, float height = 32, int resX = 8, int resY = 8,  GameObject *parent = NULL) : GameObject(id, parent)
	{
		this->zHeight = zHeight;
		this->width = width;
		this->height = height;
		this->resX = resX;
		this->resY = resY;
		this->shader = shader;
		this->modele = new ModeleHeightComponent(shader);
		PrimitiveMesh::generate_plane_terrain(modele, resX, resY, width, height);
		this->rigidbody = new RigidBody(this, true);


		this->addComponent(this->rigidbody);

		this->modele->addTexture(texture, true);
		this->modele->setHeightMap(heightMap, true, zHeight);

		this->rigidbody->generateTerrainCollision(heightMap, this->modele->getIndexedVertices(), zHeight);
	}
	~Terrain()
	{
		delete this->modele;
	}

	virtual void compute(Camera *camera, bool dfs = true)
	{
		// Collision ...
		this->modele->draw(camera, this->getTransformMatrix());

		if (dfs)
		{
			GameObject::compute(camera, dfs);
		}
	}
};

#endif