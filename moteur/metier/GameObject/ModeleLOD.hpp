#ifndef __MODELE_LOD_H__
#define __MODELE_LOD_H__

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include "GameObject.hpp"
#include "../Light/Material.hpp"
#include "../Texture.hpp"
#include "../Collision/Collision.hpp"
#include "../Shader/GlobalShader.hpp"

#include "Modele.hpp"

class ModeleLOD : public GameObject {
protected:
	//0 : High Poly, 1 : Low Poly, 2 : Impostor
	Modele* modeles[3];
	float distanceLOD[2];

public:

	// --- CONSTRUCTEURS ET DESTRUCTEURS ---
	ModeleLOD(std::string id, Modele* high, Modele* low = NULL, Modele* imposteur = NULL, GameObject* parent = NULL) : GameObject(id, parent) {
		modeles[0] = high;
		modeles[1] = low;
		modeles[2] = imposteur;
		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 100.0f;
	}

	ModeleLOD(std::string id, GlobalShader* shader, std::string fileOff, GameObject* parent = NULL) : GameObject(id, parent) {
		modeles[0] = new Modele(id, shader, fileOff);
		this->addComponent(modeles[0]->getCollision());
		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 100.0f;
	}

	ModeleLOD(std::string id, GlobalShader* shader, GameObject* parent = NULL) : GameObject(id, parent) {
		modeles[0] = new Modele(id, shader);

		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 100.0f;
	}

	ModeleLOD(std::string id, std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, GlobalShader* shader, GameObject* parent = NULL) : GameObject(id, parent) {
		modeles[0] = new Modele(id, indexed_vertices, indices, texCoords, shader);
		this->addComponent(modeles[0]->getCollision());
		distanceLOD[0] = 10.0f;
		distanceLOD[1] = 100.0f;
	}

	virtual ~ModeleLOD() {
		for (int i = 0; i < 3; i++) {
			delete modeles[i];
		}
	}

	// --- METHODES ---
	virtual void compute(Camera* camera, bool dfs = true) {
		float distance = -1.0f;
		std::vector<glm::vec3> boxCoords = modeles[0]->getCollision()->getBoundingBox()->getCoords();
		for (int i = 0; i < 8 && distance < 0.0f; i++) {
			distance = camera->isInFieldOfView(boxCoords[i]);
		}

		
		if (distance >= 0.0f) {
			int level = 2;
			if (distance <= this->distanceLOD[0]) {
				level = 0;
			}
			else if (distance <= this->distanceLOD[1]) {
				level = 1;
			}
			std::cout << "Distance : " << distance << " | Level : " << level << std::endl;
			draw(level);
		}

		if (dfs)
			GameObject::compute(camera,dfs);
	}


	void draw(int level) {
		if (modeles[level] == NULL) {
			level = 0;
		}
		std::cout << "Level : " << level << std::endl;
		modeles[level]->getShader()->use();

		glEnable(GL_TEXTURE_2D);
		modeles[level]->getShader()->drawTexture(modeles[level]->getTextureContainer().texture, modeles[level]->getTextureContainer().id);

		//LIGHT TEST
		modeles[level]->getShader()->setLightTest();
		modeles[level]->getShader()->drawMesh(modeles[level]->getVAO(), modeles[level]->getIndices().size(), this->getTransformMatrix(), modeles[level]->getStaticMaterial());
	}


	// ---- GETTER ET SETTER ---

	Modele* getModele(int level) {
		return this->modeles[level];
	}
};





#endif