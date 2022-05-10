#ifndef __H_TERRAIN__
#define __H_TERRAIN__

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
#include <Tools/PrimitiveMesh.hpp>

#include "GameObject.hpp"
#include <GameObject/ModeleHeightComponent.hpp>

class Terrain : public GameObject
{
private:
	BulletRigidbody *terrainRigid;

	GlobalShaderExtended *shader;
	Material material;
	float maxHeight;	 // Hauteur max de la heightmap;
	float width, height; // Taille plan
	int res;			 // Résolution
	int multiplicatorCollision = 4;

protected:
	ModeleHeightComponent *modele;

public:
	Terrain(std::string id, GlobalShaderExtended *shader, Texture *texture, Texture *heightMap, float maxHeight = 1.0f, float unite = 1.0f, int resolution = 8, GameObject *parent = NULL) : GameObject(id, parent)
	{
		this->maxHeight = maxHeight;
		this->width = heightMap->getWidth() * unite;
		this->height = heightMap->getHeight() * unite;
		this->res = (int)(((float)heightMap->getWidth() / 32.0f) * resolution);
		this->shader = shader;
		this->modele = new ModeleHeightComponent(shader, this->maxHeight);
		this->shader->setMaxHeight(maxHeight);
		PrimitiveMesh::generate_plane_terrain(modele, res, res, this->width, this->height);

		this->modele->addTexture(texture, true);
		this->modele->setHeightMap(heightMap, true);

		terrainRigid = new BulletRigidbody();
		terrainRigid->setToHeightTerrain(heightMap, this->maxHeight, this->width);
		generateRigidbodyTransform(terrainRigid);
	}

	BulletRigidbody *getRigidbody()
	{
		return this->terrainRigid;
	}

	~Terrain()
	{
		delete this->modele;
	}

	virtual void compute(CameraData *data, Lightning *lights, bool dfs = true)
	{
		this->modele->draw(data, lights, this->getTransformMatrix(), this->getPosition());

		if (dfs)
		{
			GameObject::compute(data, lights, dfs);
		}
	}
};

#endif