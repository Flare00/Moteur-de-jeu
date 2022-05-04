#ifndef __H_TERRAIN__
#define __H_TERRAIN__

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <common/shader.hpp>
#include <common/objloader.hpp>

#include <Light/Material.hpp>
#include <Texture.hpp>
#include <Collision/Collision.hpp>
#include <Shader/GlobalShader.hpp>
#include <Tools/SimplifyMesh.hpp>

#include "GameObject.hpp"
#include <GameObject/ModeleComponent.hpp>

class Terrain : public GameObject
{
protected:
	ModeleComponent *modele;
	Texture *texture;
	// RIGIDBODY (collision)

	/*
	private:
		GlobalShader* shader;
		Material material;
	*/

public:
	Terrain()
	{
		
	}
	~Terrain()
	{

	}
	
};

#endif