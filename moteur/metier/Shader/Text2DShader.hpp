#ifndef __TEXT2D_SHADER_H__
#define __TEXT2D_SHADER_H__

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Texture.hpp"
#include <Global.hpp>
#include <Tools/LoadShader.hpp>
class Text2DShader
{
protected:
	GLuint id;
	std::string vertex;
	std::string fragment;

	GLuint u_projection;
	GLuint u_text;
	GLuint u_textColor;

	glm::mat4 projection;

public:
	Text2DShader(std::string vertex, std::string fragment, glm::mat4 ortho)
	{
		this->id = LoadShaders(vertex.c_str(), fragment.c_str());
		this->vertex = vertex;
		this->fragment = fragment;

		glUseProgram(id);
		this->u_projection = glGetUniformLocation(this->id, "u_projection");
		this->u_text = glGetUniformLocation(this->id, "u_text");
		this->u_textColor = glGetUniformLocation(this->id, "u_textColor");
	}

	virtual ~Text2DShader()
	{
		glDeleteProgram(id);
	}

	void use()
	{
		glUseProgram(id);
	}

	void draw(GLuint VAO, GLsizei nbVertex, Texture *texture)
	{
		glUseProgram(id);
		glUniformMatrix4fv(this->u_projection, 1, GL_FALSE, &this->projection[0][0]);
		glUniform3fv(this->u_textColor, 1, &glm::vec3(1, 0, 0)[0]);

		texture->draw(this->u_text, 0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, nbVertex);

		glDisable(GL_BLEND);
	}

	GLuint getProgramID()
	{
		return this->id;
	}
};

#endif
