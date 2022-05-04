#ifndef __MODELE_COMPONENT_HPP__
#define __MODELE_COMPONENT_HPP__

#include <common/objloader.hpp>

#include <vector>
#include <glm/glm.hpp>

#include <Shader/GlobalShader.hpp>
#include <Component.hpp>
#include <Texture.hpp>
#include <Collision/RigidBody.hpp>

class ModeleComponent : public Component
{
private:
	GlobalShader *shader;

protected:
	// Modele
	std::vector<glm::vec3> vertexArray;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	// Material
	Material material;

	// Texture
	struct TextureContainer
	{
		Texture *texture = NULL;
		bool destroyAtEnd;
	};
	std::vector<TextureContainer> textures;
	int heightMapId;

	// Buffers
	GLuint VAO;
	GLuint VBO[3];
	GLuint EBO;

	//---
	bool hasData;

public:
	// --- CONSTRUCTEURS ET DESTRUCTEURS ---

	ModeleComponent(GlobalShader *shader, std::string fileOff) : Component(Component::Type::MODELE)
	{
		this->shader = shader;
		std::vector<unsigned short> indices; // Triangles concat�n�s dans une liste
		std::vector<std::vector<unsigned short>> triangles;

		// Chargement du fichier de maillage
		loadOFF(fileOff, this->vertexArray, indices, triangles);
		for (int i = 0, max = indices.size(); i < max; i++)
		{
			this->indices.push_back((int)indices[i]);
		}
		loadBuffer();
	}

	ModeleComponent(GlobalShader *shader) : Component(Component::Type::MODELE)
	{
		this->shader = shader;
	}

	ModeleComponent(GlobalShader *shader, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> normals, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords) : Component(Component::Type::MODELE)
	{
		this->shader = shader;
		this->normals = normals;
		this->vertexArray = indexed_vertices;
		this->indices = indices;
		this->texCoords = texCoords;
		loadBuffer();
	}

	//--- BUFFERS ---
	virtual void loadBuffer()
	{
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		if (this->texCoords.size() > 0)
		{
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, this->texCoords.size() * sizeof(vec2), &this->texCoords[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		}
		if (this->normals.size() > 0)
		{
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(vec3), &this->normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

		this->hasData = true;
	}

	virtual void draw(Camera *camera, glm::mat4 transform, RigidBody *rigidbody = NULL)
	{
		if (!hasData)
		{
			return;
		}
		bool isInFOV = false;

		if (rigidbody != NULL)
		{
			std::vector<glm::vec3> boxCoords = rigidbody->getCollision()->getBoundingBox()->getCoords();
			for (int i = 0, max = boxCoords.size(); i < max && !isInFOV; i++)
			{
				isInFOV = camera->isInFieldOfView(boxCoords[i]);
			}
		}
		else
		{
			isInFOV = true; // TODO trouver une solution
		}

		if (!isInFOV)
			return;

		this->shader->use();

		glEnable(GL_TEXTURE_2D);

		for (int i = 0, max = textures.size(); i < max; i++)
		{
			this->shader->drawTexture(this->textures[i].texture, i);
		}

		// LIGHT TEST
		this->shader->setLight();
		this->shader->drawMaterial(this->material);
		this->shader->drawMesh(this->VAO, this->indices.size(), transform);
	}

	Texture *getTexture(int i)
	{
		return this->textures[i].texture;
	}

	std::vector<TextureContainer> getTexturesContainers()
	{
		return this->textures;
	}

	TextureContainer getTextureContainer(int i)
	{
		return this->textures[i];
	}

	void addTexture(Texture *texture, bool destroyAtEnd)
	{
		TextureContainer tc;
		tc.texture = texture;
		tc.destroyAtEnd = destroyAtEnd;
		this->textures.push_back(tc);
	}

	void removeTexture(int i)
	{
		if (i > 0 && i < this->textures.size())
		{
			this->textures.erase(this->textures.begin() + i);
		}
	}

	void setData(std::vector<glm::vec3> indexed_vertices, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, std::vector<glm::vec3> normals)
	{
		this->vertexArray = indexed_vertices;
		this->normals = normals;
		this->indices = indices;
		this->texCoords = texCoords;
		loadBuffer();
	}
	std::vector<glm::vec3> getIndexedVertices()
	{
		return this->vertexArray;
	}

	std::vector<glm::vec3> getNormals()
	{
		return this->normals;
	}
	GlobalShader *getShader()
	{
		return this->shader;
	}
	std::vector<unsigned int> getIndices()
	{
		return this->indices;
	}

	std::vector<glm::vec2> getTexCoords()
	{
		return this->texCoords;
	}

	Material *getMaterial()
	{
		return &this->material;
	}

	Material getStaticMaterial()
	{
		return this->material;
	}

	void setMaterial(Material material)
	{
		this->material = material;
	}

	GLuint getVAO()
	{
		return this->VAO;
	}
};

#endif