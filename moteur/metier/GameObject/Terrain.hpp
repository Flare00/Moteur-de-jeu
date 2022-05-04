#ifndef __H_TERRAIN__
#define __H_TERRAIN__

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <common/objloader.hpp>
#include <Light/Material.hpp>
#include <Texture.hpp>
#include <Collision/Collision.hpp>
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

protected:
	ModeleHeightComponent *modele;
	// RIGIDBODY (collision)

public:
	Terrain(std::string id, GlobalShaderExtended *shader, Texture *texture, Texture *heightMap, float width = 64, float height = 64, int resX = 64, int resY = 64, GameObject *parent = NULL) : GameObject(id, parent)
	{
		this->width = width;
		this->height = height;
		this->resX = resX;
		this->resY = resY;
		this->shader = shader;
		this->modele = new ModeleHeightComponent(shader);
		PrimitiveMesh::generate_plane(modele, resX, resY, width, height);
		this->modele->addTexture(texture, true);
		this->modele->setHeightMap(heightMap, true);
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