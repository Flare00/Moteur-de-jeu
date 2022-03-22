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
private:
	GlobalShader* shader;
	Material material;
protected:
	//0 : High Poly, 1 : Low Poly, 2 : Impostor
	Modele* modeles[3];
	float distanceLOD[2];
	//Textures
	int lastTextureID = 0;

	struct TextureContainer {
		int id = -1;
		Texture* texture = NULL;
		bool destroyAtEnd;
	};

	TextureContainer texture;
	//Collision
	Collision* collision;

	bool hasData;

public:

	// --- CONSTRUCTEURS ET DESTRUCTEURS ---
	ModeleLOD(std::string id, GlobalShader* shader, std::string fileOff, GameObject* parent = NULL) : GameObject(id, parent) {
		Modele[0] = new Modele(id, shader, fileOff);
		distanceLOD[0] = 100.0f;
		distanceLOD[1] = 1000.0f;
	}

	ModeleLOD(std::string id, GlobalShader* shader, GameObject* parent = NULL) : GameObject(id, parent) {
		Modele[0] = new Modele(id, shader);
		distanceLOD[0] = 100.0f;
		distanceLOD[1] = 1000.0f;
	}

	ModeleLOD(std::string id, std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, GlobalShader* shader, GameObject* parent = NULL) : GameObject(id, parent) {
		Modele[0] = new Modele(id, indexed_vertices, indices, texCoords, shader);
		distanceLOD[0] = 100.0f;
		distanceLOD[1] = 1000.0f;
	}

	virtual ~ModeleLOD() {
		for (int i = 0; i < 3; i++) {
			delete Modele[i];
		}
	}

	// --- METHODES ---
	void drawTextures() {
		//Draw les textures

	}
	void drawMesh(bool uv) {
		//Set Transformation in shader		

	}

	virtual void compute(Camera* camera, bool dfs = true) {
		bool isInFOV = false;
		glm::vec3* boxCoords = this->collision->getBoundingBox()->getCoords();
		for (int i = 0; i < 8 && !isInFOV; i++) {
			isInFOV = camera->isInFieldOfView(boxCoords[i]);
		}
		
		if (isInFOV) {
			float dist = camera->getDistance(this->collision->getBoundingBox()->getCenter());
			int level = 2;
			if (dist < this->distanceLOD[0]) {
				level = 0;
			}
			else if (dist < this->distanceLOD[1]) {
				level = 1;
			}
			draw(level);
		}

		if (dfs)
			GameObject::draw(dfs);
	}


	void draw(int level) {

		if (!hasData) {
			return;
		}
		this->shader->use();

		glEnable(GL_TEXTURE_2D);

		this->shader->drawTexture(this->texture.texture, this->texture.id);

		//LIGHT TEST
		this->shader->setLightTest();
		this->shader->drawMesh(this->VAO, this->indices.size(), this->getTransformMatrix(), this->material);

	}

	void fillTextureData(TextureContainer* container, Texture* texture, bool destroyAtEnd) {
		if (container->id == -1) {
			container->id = lastTextureID++;
		}
		container->texture = texture;
		container->destroyAtEnd = destroyAtEnd;
	}

	// ---- GETTER ET SETTER ---

	Texture* getTexture() {
		return this->texture.texture;
	}

	void setTexture(Texture* texture, bool destroyAtEnd) {
		this->fillTextureData(&this->texture, texture, destroyAtEnd);

	}

	void removeTexture() {
		this->texture.texture = NULL;
	}

	std::vector<glm::vec3> getIndexedVertices() {
		return this->vertexArray;
	}

	Collision* getCollision(int level) {
		return this->modeles[level]->collision;
	}

	Shader* getShader() {
		return this->shader;
	}

	std::vector<unsigned int> getIndices() {
		return this->indices;
	}

	std::vector<glm::vec2> getTexCoords() {
		return this->texCoords;
	}

	Material* getMaterial() {
		return &this->material;
	}
};





#endif