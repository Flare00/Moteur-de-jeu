#ifndef __H_MODEL__
#define __H_MODEL__
#define _USE_MATH_DEFINES

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <common/shader.hpp>
#include <common/objloader.hpp>

#include "GameObject.hpp"
#include "../World/Camera.hpp"
#include "../Light/Material.hpp"
#include "../Texture.hpp"
#include <Collision/RigidBody.hpp>
#include "../Shader/GlobalShader.hpp"

class Modele : public GameObject {
public: 
	struct TextureContainer {
		int id = -1;
		Texture* texture = NULL;
		bool destroyAtEnd;
	};
private:
	GlobalShader* shader;
	Material material;
protected:
	std::vector<glm::vec3> vertexArray;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	//Textures
	int lastTextureID = 0;

	TextureContainer texture;

	//COLLISION
	RigidBody* rigidbody = NULL;

	//Buffers
	GLuint VAO;
	GLuint VBO[3];
	GLuint EBO;

	//---
	bool hasData;

public:

	//--- BUFFERS ---
	virtual void loadBuffer() {
		bool notEmptyTexture = (this->texCoords.size() > 0);
		bool notEmptyNormal = (this->normals.size() > 0);

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(3, this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(VAO);

		// position attribute
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, this->vertexArray.size() * sizeof(vec3), &this->vertexArray[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		if (this->texCoords.size() > 0) {
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, this->texCoords.size() * sizeof(vec2), &this->texCoords[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		if (this->normals.size() > 0) {
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(vec3), &this->normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

		hasData = true;
	}

	// --- CONSTRUCTEURS ET DESTRUCTEURS ---

	Modele(std::string id, GlobalShader* shader, std::string fileOff, GameObject* parent = NULL) : GameObject(id, parent) {
		this->shader = shader;
		std::vector<unsigned short> indices; //Triangles concat�n�s dans une liste
		std::vector<std::vector<unsigned short> > triangles;

		//Chargement du fichier de maillage
		loadOFF(fileOff, this->vertexArray, indices, triangles);
		for (int i = 0, max = indices.size(); i < max; i++) {
			this->indices.push_back((int)indices[i]);
		}
		loadBuffer();
	}

	Modele(std::string id, GlobalShader* shader, GameObject* parent = NULL) : GameObject(id, parent) {
		this->shader = shader;
	}

	Modele(std::string id, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> normals, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, GlobalShader* shader, GameObject* parent = NULL) : GameObject(id, parent) {
		this->shader = shader;
		this->normals = normals;
		this->vertexArray = indexed_vertices;
		this->indices = indices;
		this->texCoords = texCoords;

		loadBuffer();
	}

	void setRigidBody(RigidBody* body) {
		if(this->rigidbody != NULL)
		{
			delete this->rigidbody;
		}
		this->rigidbody = body;

		this->addComponent(this->rigidbody);

		if (this->vertexArray.size() > 0) 
		{
			this->rigidbody->generateCollision(this->vertexArray);
		}
	}

	virtual ~Modele() {
		if (this->texture.destroyAtEnd && this->texture.texture != NULL) {
			free(this->texture.texture);
		}

		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(3, this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}

	// ---- METHODE ----
	virtual void compute(Camera* camera, bool dfs = true) {
		bool isInFOV = false;
		std::vector<glm::vec3> boxCoords = this->rigidbody->getCollision()->getBoundingBox()->getCoords();
		for (int i = 0, max = boxCoords.size(); i < max && !isInFOV; i++) {
			isInFOV = camera->isInFieldOfView(boxCoords[i]);
		}
		if (isInFOV) {
			draw();
		}

		if (dfs)
			GameObject::compute(camera, dfs);
	}
	void draw() {
		if (!hasData) {
			return;
		}
		this->shader->use();

		glEnable(GL_TEXTURE_2D);

		this->shader->drawTexture(this->texture.texture, this->texture.id);

		//LIGHT TEST
		this->shader->setLight();
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

	TextureContainer getTextureContainer() {
		return this->texture;
	}

	void setTexture(Texture* texture, bool destroyAtEnd) {
		this->fillTextureData(&this->texture, texture, destroyAtEnd);

	}

	void removeTexture() {
		this->texture.texture = NULL;
	}

	void setData(std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, std::vector<glm::vec3> normals) {
		this->vertexArray = indexed_vertices;
		this->normals = normals;
		this->indices = indices;
		this->texCoords = texCoords;
		loadBuffer();
	}

	std::vector<glm::vec3> getIndexedVertices() {
		return this->vertexArray;
	}

	std::vector<glm::vec3> getNormals() {
		return this->normals;
	}

	RigidBody* getRigidBody() {
		return this->rigidbody;
	}
	void setCollisionState(bool coll) {
		this->rigidbody->getCollision()->setActif(coll);
	}

	GlobalShader* getShader() {
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

	Material getStaticMaterial() {
		return this->material;
	}

	GLuint getVAO() {
		return this->VAO;
	}

};





#endif