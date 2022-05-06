#ifndef __DEBUG_SHADER_H__
#define __DEBUG_SHADER_H__

#include <string>
#include <vector>

#include "Shader.hpp"
#include "Transformation.hpp"


class DebugShader : public Shader
{
protected:
	
	GLuint u_model;
	GLuint u_color;

public:
	DebugShader(std::string vertex, std::string fragment, std::string geometry = "", std::string tessControl = "", std::string tessEval = "") : Shader(vertex, fragment, geometry, tessControl, tessEval)
	{
		this->u_model = glGetUniformLocation(this->id, "u_model");
		this->u_color = glGetUniformLocation(this->id, "u_color");
	}

	virtual void drawMesh(GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix, glm::vec3 color)
	{
		glUniform3fv(this->u_color,1, &color[0]);

		// Load Vertex
		glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);

		// Bind VAO
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, size_indice);
	}
};

#endif // !__GLOBAL_SHADER_H__
