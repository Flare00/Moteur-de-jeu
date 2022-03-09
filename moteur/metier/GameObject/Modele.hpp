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
private :
	GlobalShader* shader;
protected:
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<float> indexed_vtc;
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
	GLuint vertexbuffer;
	GLuint texcoordbuffer;
	GLuint elementbuffer;

	//---
	bool hasData;

public:

	//--- BUFFERS ---
	virtual void loadBuffer() {
		//Load vertex Info
		glGenBuffers(1, &this->vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, this->indexed_vertices.size() * sizeof(glm::vec3), &this->indexed_vertices[0], GL_STATIC_DRAW);


		//Load TexCoords Info
		glGenBuffers(1, &this->texcoordbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->texcoordbuffer);
		glBufferData(GL_ARRAY_BUFFER, this->texCoords.size() * sizeof(glm::vec2), &this->texCoords[0], GL_STATIC_DRAW);


		//Load Element Info
		glGenBuffers(1, &this->elementbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->elementbuffer);
		glBufferData(GL_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
		hasData = true;
	}

	// --- CONSTRUCTEURS ET DESTRUCTEURS ---

	Modele(std::string id, GlobalShader* shader, std::string fileOff, GameObject* parent = NULL) : GameObject(id, parent) {
		this->shader = shader;
		std::vector<unsigned short> indices; //Triangles concat�n�s dans une liste
		std::vector<std::vector<unsigned short> > triangles;

		//Chargement du fichier de maillage
		loadOFF(fileOff, this->indexed_vertices, indices, triangles);
		for (int i = 0, max = indices.size(); i < max; i++) {
			this->indices.push_back((int)indices[i]);
		}
		this->boundingBox = new BoundingBox(this->indexed_vertices);

		loadBuffer();
	}

	Modele(std::string id, GlobalShader * shader, GameObject* parent = NULL) : GameObject(id, parent) {
		this->shader = shader;
	}

	Modele(std::string id, std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords,GlobalShader * shader, GameObject* parent = NULL) : GameObject(id, parent) {
		this->shader = shader;
		this->indexed_vertices = indexed_vertices;
		this->indices = indices;
		this->texCoords = texCoords;
		this->boundingBox = new BoundingBox(this->indexed_vertices);

		loadBuffer();
	}

	virtual ~Modele() {
		if (this->texture.destroyAtEnd && this->texture.texture != NULL) {
			free(this->texture.texture);
		}

		glDeleteBuffers(1, &this->vertexbuffer);
		glDeleteBuffers(1, &this->texcoordbuffer);
		glDeleteBuffers(1, &this->elementbuffer);
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
		this->shader->drawMesh(this->vertexbuffer, this->texcoordbuffer, this->elementbuffer, &this->indices, this->getTransformMatrix());

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
		this->indexed_vertices = indexed_vertices;
		this->indices = indices;
		this->texCoords = texCoords;

		this->boundingBox = new BoundingBox(this->indexed_vertices);

		loadBuffer();
	}

	std::vector<glm::vec3> getIndexedVertices() {
		return this->indexed_vertices;
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
		float parallele = ( M_PI * (float)j) / ((float)nbParalleles);
		for (int i = 0; i<= nbMeridien; i++) {
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