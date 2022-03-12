#ifndef __H_TERRAIN__
#define __H_TERRAIN__

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>


#include "Modele.hpp"
#include "../Shader/TerrainShader.hpp"

class Terrain : public Modele {
private:
	TerrainShader* shader;
protected:
	//HeightMap
	TextureContainer heightMap;
	TextureContainer textureTop;
	TextureContainer textureBottom;

	float limitTop;
	float limitBottom;
	float limitBlur = 0.1;

public :
	Terrain(std::string id, TerrainShader * shader, std::string fileOff, GameObject* parent = NULL) : Modele(id, shader, fileOff, parent) {
		this->shader = shader;
	}
	Terrain(std::string id, TerrainShader* shader, GameObject* parent = NULL) : Modele(id, shader, parent) {
		this->shader = shader;
	}
	Terrain(std::string id, std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, TerrainShader* shader, GameObject* parent = NULL)
		: Modele(id, indexed_vertices, indices, texCoords,  shader, parent) 
	{
		this->shader = shader;
	}




	virtual ~Terrain() {
		if (heightMap.texture != NULL && heightMap.destroyAtEnd) {
			free(heightMap.texture);
		}
		if (textureTop.texture != NULL && textureTop.destroyAtEnd) {
			free(textureTop.texture);
		}
		if (textureBottom.texture != NULL && textureBottom.destroyAtEnd) {
			free(textureBottom.texture);
		}
	}

	virtual void loadBuffer() {
		Modele::loadBuffer();
	}

	void setHeightMap(Texture* tex, bool destroyAtEnd = false) {
		this->fillTextureData(&this->heightMap, tex, destroyAtEnd);
	}

	void setTextureTop(Texture* tex, float limitTop = 0.7f, bool destroyAtEnd = false) {
		this->limitTop = limitTop;
		this->fillTextureData(&this->textureTop, tex, destroyAtEnd);
	}

	void setTextureBottom(Texture* tex, float limitBottom = 0.3f, bool destroyAtEnd = false) {
		this->limitBottom = limitBottom;
		this->fillTextureData(&this->textureBottom, tex, destroyAtEnd);
	}


	void setLimitTop(float limit) {
		this->limitTop = limit;
	}
	void setLimitBottom(float limit) {
		this->limitBottom = limit;
	}
	void setLimitBlur(float limit) {
		this->limitBlur = limit;
	}

	virtual void draw(bool dfs = true) {
		if (!this->hasData) {
			return;
		}
		this->shader->use();

		glEnable(GL_TEXTURE_2D);

		//Draw terrain information
		this->shader->drawTextureTop(this->textureTop.texture, this->textureTop.id);
		this->shader->drawTexture(this->texture.texture, this->texture.id);
		this->shader->drawTextureBottom(this->textureBottom.texture, this->textureBottom.id);

		this->shader->drawHeightMap(this->heightMap.texture, this->heightMap.id);
		
		this->shader->setLimit(this->limitTop, this->limitBottom, this->limitBlur);
		//Draw Modele information
		this->shader->drawMesh(this->VAO, this->indices.size(), this->getTransformMatrix());

		if(dfs)
			GameObject::draw(dfs);
	}
};

#endif