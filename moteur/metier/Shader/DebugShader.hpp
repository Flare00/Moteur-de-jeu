#ifndef __DEBUG_SHADER_H__
#define __DEBUG_SHADER_H__

#include <string>
#include <vector>

#include "Shader.hpp"

class DebugShader : public Shader
{
protected:

	GLuint u_color;

public:
	DebugShader(std::string vertex, std::string fragment, std::string geometry = "", std::string tessControl = "", std::string tessEval = "") : Shader(vertex, fragment, geometry, tessControl, tessEval)
	{
		this->u_color = glGetUniformLocation(this->id, "u_color");
	}

	virtual void drawMesh(GLuint VAO, GLsizei size_indice, glm::vec3 color)
	{
		glUseProgram(id);
		glUniform3fv(this->u_color,1, &color[0]);

		glLineWidth(1.0f);

		// Bind VAO
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, size_indice);
	}
};

#endif // !__GLOBAL_SHADER_H__
