#ifndef __SHADOW_SHADER_HPP__
#define __SHADOW_SHADER_HPP__

#include <stdio.h>
#include <string>
#include <sstream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>

#include <World/Camera.hpp>
#include <Tools/LoadShader.hpp>
class ShadowShader
{
protected:
	GLuint id;
	std::string vertex;
	std::string fragment;

	GLuint u_view;
	GLuint u_projection;
	GLuint u_model;

	GLuint u_light_matrix;

	GLuint u_is_tesselate;
	GLuint u_is_terrain;
	GLuint u_heightmap;
	GLuint u_max_height;

	bool hasHeightMap = false;
	float maxHeight = 1.0f;

public:
	ShadowShader(std::string vertex, std::string fragment, std::string geometry = "", std::string tessControl = "", std::string tessEval = "")
	{
		this->id = LoadShaders(vertex, fragment, geometry, tessControl, tessEval);
		this->vertex = vertex;
		this->fragment = fragment;

		glUseProgram(id);
		this->u_light_matrix = glGetUniformLocation(this->id, "u_light_matrix");
		this->u_model = glGetUniformLocation(this->id, "u_model");
		/*this->u_projection = glGetUniformLocation(this->id, "u_projection");
		this->u_view = glGetUniformLocation(this->id, "u_view");
		;*/


		/*this->u_is_tesselate = glGetUniformLocation(this->id, "u_is_tesselate");
		this->u_is_terrain = glGetUniformLocation(this->id, "u_is_terrain");
		this->u_heightmap = glGetUniformLocation(this->id, "u_heightmap");
		this->u_max_height = glGetUniformLocation(this->id, "u_max_height");*/
	}

	virtual ~ShadowShader()
	{
		glDeleteProgram(id);
	}

	void use()
	{
		glUseProgram(id);
	}

	/*void drawView(glm::mat4 projection, glm::mat4 view)
	{
		glUseProgram(id);
		glUniformMatrix4fv(this->u_projection, 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(this->u_view, 1, GL_FALSE, &view[0][0]);
	}*/

	void drawView(glm::mat4 lightMatrix)
	{
		glUseProgram(id);
		glUniformMatrix4fv(this->u_light_matrix, 1, GL_FALSE, &lightMatrix[0][0]);
	}

	void drawHeightMap(Texture* texture, int id, float maxHeight)
	{
		if (texture != NULL)
		{
			this->maxHeight = maxHeight;
			texture->draw(this->u_heightmap, id);
			hasHeightMap = true;
		}
	}

	void drawMesh(GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix)
	{
		/*
		glUniform1i(this->u_is_tesselate, (hasHeightMap ? 1 : 0));
		glUniform1i(this->u_is_terrain, (hasHeightMap ? 1 : 0));
		glUniform1i(this->u_max_height, maxHeight);*/

		// Bind VAO
		glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, size_indice, GL_UNSIGNED_INT, 0);
		hasHeightMap = false;
	}

	void drawPatch(GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix)
	{
		/*
		glUniform1i(this->u_is_tesselate, (hasHeightMap ? 1 : 0));
		glUniform1i(this->u_is_terrain, (hasHeightMap ? 1 : 0));
		glUniform1i(this->u_max_height, maxHeight);*/

		// Bind VAO
		glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);
		glBindVertexArray(VAO);
		glDrawArrays(GL_PATCHES, 0, size_indice);
		hasHeightMap = false;
	}

	GLuint getProgramID()
	{
		return this->id;
	}
};


#endif