#ifndef __TEXT_2D_HPP__
#define __TEXT_2D_HPP__

#include <iostream>

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader/Text2DShader.hpp>

using namespace glm;

class Text2D {
private:

	GLuint VAO;
	GLuint VBO[2];

	Text2DShader* shader;
	Texture* texture;

	int textureId = 0;
	int widthChar, heightChar;
	int nbCharLine, nbCharCol;
	float invNbCharLine, invNbCharCol;
public:
	Text2D(Text2DShader* shader, Texture* atlas, int widthChar, int heightChar) {
		this->shader = shader;
		this->texture = atlas;
		this->widthChar = widthChar;
		this->heightChar = heightChar;
		this->nbCharLine = this->texture->getWidth() / widthChar;
		this->nbCharCol = this->texture->getHeight() / heightChar;
		this->invNbCharLine = 1.0f / (float)this->nbCharLine;
		this->invNbCharCol = 1.0f / (float)this->nbCharCol;

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(2, this->VBO);

	}

	void DrawText(std::string texte, float x, float y) {
		this->shader->use();
		std::vector<vec2> vertices;
		std::vector<vec2> uv;
		float sizeX = 0.05f;
		float sizeY = 2 * sizeX;
		for (int i = 0, max = texte.length(); i < max; i++) {
			glm::vec2 vTL = glm::vec2(x + (i * sizeX), y );
			glm::vec2 vTR = glm::vec2(x + (i * sizeX) + sizeX, y );
			glm::vec2 vBR = glm::vec2(x + (i * sizeX) + sizeX, y - sizeY);
			glm::vec2 vBL = glm::vec2(x + (i * sizeX), y - sizeY);

			vertices.push_back(vTL);
			vertices.push_back(vBL);
			vertices.push_back(vTR);
			vertices.push_back(vBR);
			vertices.push_back(vTR);
			vertices.push_back(vBL);

			int numChar = texte.at(i) - 0x20;
			float uv_x = (float)(numChar % this->nbCharLine) / (float)this->nbCharLine;
			float uv_y = (float)((int)(numChar / this->nbCharLine)) / (float)this->nbCharCol;

			glm::vec2 uvTL = glm::vec2(uv_x, uv_y);
			glm::vec2 uvTR = glm::vec2(uv_x + invNbCharLine, uv_y);
			glm::vec2 uvBR = glm::vec2(uv_x + invNbCharLine, uv_y + invNbCharCol);
			glm::vec2 uvBL = glm::vec2(uv_x, uv_y + invNbCharCol);

			uv.push_back(uvTL);
			uv.push_back(uvBL);
			uv.push_back(uvTR);
			uv.push_back(uvBR);
			uv.push_back(uvTR);
			uv.push_back(uvBL);
		}
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(vec2), &uv[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		this->shader->draw(this->VAO, vertices.size(), this->texture);
	}

	~Text2D() {
		// Delete buffers
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(2, this->VBO);
	}
};

#endif