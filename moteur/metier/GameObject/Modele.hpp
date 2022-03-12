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
#include <common/vboindexer.hpp>

#include "GameObject.hpp"

#include "../Texture.hpp"
#include "../Collision/BoundingBox.hpp"
#include "../Shader/GlobalShader.hpp"

class Modele : public GameObject {
private:
	GlobalShader* shader;
protected:
	std::vector<glm::vec3> vertexArray;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	//Textures
	int lastTextureID = 0;

	struct TextureContainer {
		int id = -1;
		Texture* texture = NULL;
		bool destroyAtEnd;
	};



	TextureContainer texture;

	//BoundingBox
	BoundingBox* boundingBox;

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
		this->boundingBox = new BoundingBox(this->vertexArray);

		loadBuffer();
	}

	Modele(std::string id, GlobalShader* shader, GameObject* parent = NULL) : GameObject(id, parent) {
		this->shader = shader;
	}

	Modele(std::string id, std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, GlobalShader* shader, GameObject* parent = NULL) : GameObject(id, parent) {
		this->shader = shader;
		this->vertexArray = indexed_vertices;
		this->indices = indices;
		this->texCoords = texCoords;
		this->boundingBox = new BoundingBox(this->vertexArray);

		loadBuffer();
	}

	virtual ~Modele() {
		if (this->texture.destroyAtEnd && this->texture.texture != NULL) {
			free(this->texture.texture);
		}

		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(3, this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}

	// --- METHODES ---
	void drawTextures() {
		//Draw les textures

	}
	void drawMesh(bool uv) {
		//Set Transformation in shader		

	}


	virtual void draw(bool dfs = true) {
		if (!hasData) {
			return;
		}
		this->shader->use();

		glEnable(GL_TEXTURE_2D);
		this->shader->drawTexture(this->texture.texture, this->texture.id);
		this->shader->drawMesh(this->VAO, this->indices.size(), this->getTransformMatrix());

		if (dfs)
			GameObject::draw(dfs);
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


	void setData(std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords) {
		this->vertexArray = indexed_vertices;
		this->indices = indices;
		this->texCoords = texCoords;

		this->boundingBox = new BoundingBox(this->vertexArray);

		loadBuffer();
	}

	std::vector<glm::vec3> getIndexedVertices() {
		return this->vertexArray;
	}

	BoundingBox* getBoundingBox() {
		return this->boundingBox;
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
};


void generate_plane(Modele* modele, int res_x, int res_y, float size_x, float size_y) {
	std::vector<glm::vec3> indexed_vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec2> texCoords;

	for (int i = 0; i < res_x; i++) {
		float pointX = (float)i * (size_x / (float)res_x);
		for (int j = 0; j < res_y; j++) {
			float pointY = (float)j * (size_y / (float)res_y);
			indexed_vertices.push_back(glm::vec3(pointX - (size_x / 2.0), 0.0f, pointY - (size_y / 2.0)));
			texCoords.push_back(glm::vec2(((float)i) / ((float)res_x - 1), ((float)j) / ((float)res_y - 1)));
		}
	}


	for (int i = 0; i < res_x - 1; i++) {
		for (int j = 0; j < res_y - 1; j++) {
			indices.push_back((i * res_y) + j);
			indices.push_back((i * res_y) + j + 1);
			indices.push_back(((i + 1) * res_y) + j);

			indices.push_back((i * res_y) + j + 1);
			indices.push_back(((i + 1) * res_y) + j + 1);
			indices.push_back(((i + 1) * res_y) + j);
		}
	}

	modele->setData(indexed_vertices, indices, texCoords);
}

void generate_uv_sphere(Modele* modele, int nbMeridien, int nbParalleles) {
	std::vector<glm::vec3> indexed_vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec2> texCoords;
	for (int j = 0; j <= nbParalleles; j++) {
		float parallele = (M_PI * (float)j) / ((float)nbParalleles);
		for (int i = 0; i <= nbMeridien; i++) {
			//Coordinate
			float meridien = (2.0f * M_PI * (float)i) / ((float)nbMeridien);
			float x = sin(parallele) * cos(meridien);
			float y = cos(parallele);
			float z = sin(parallele) * sin(meridien);
			indexed_vertices.push_back(glm::vec3(x, y, z));

			//Texture Coordinate
			texCoords.push_back(glm::vec2(meridien / (2.0f * M_PI), parallele / (M_PI)));

			if (j < nbParalleles && i < nbMeridien) {
				//Triangles
				int p1 = (j * (nbMeridien + 1)) + i;
				int p2 = p1 + (nbMeridien + 1);

				indices.push_back(p1);
				indices.push_back(p2);
				indices.push_back(p1 + 1);

				indices.push_back(p1 + 1);
				indices.push_back(p2);
				indices.push_back(p2 + 1);
			}
		}
	}
	modele->setData(indexed_vertices, indices, texCoords);
}


#endif