#ifndef __GLOBAL_SHADER_H__
#define __GLOBAL_SHADER_H__

#include "Shader.hpp"
#include "Transformation.hpp"
#include "Texture.hpp"
class GlobalShader : public Shader {
protected:
	//vertex
	GLuint u_model;

	//fragment
	GLuint u_texture;
	GLuint u_has_texture;
public:
	GlobalShader(std::string vertex, std::string fragment) : Shader(vertex, fragment) {
		this->u_model = glGetUniformLocation(this->id, "u_model");
		this->u_texture = glGetUniformLocation(this->id, "u_texture");
		this->u_has_texture = glGetUniformLocation(this->id, "u_has_texture");

	}

	void drawTexture(Texture* texture, int id) {
		if (texture != NULL) {
			texture->draw(this->u_texture, id);
			glUniform1i(this->u_has_texture, 1);
		}
		else {
			glUniform1i(this->u_has_texture, 0);
		}
	}

	void drawMesh(GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix) {
		//Load Vertex
		glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);

		// Bind VAO
		glBindVertexArray(VAO);

		// Draw
		glDrawElements(GL_TRIANGLES, size_indice, GL_UNSIGNED_INT, 0);
	}
};


#endif // !__GLOBAL_SHADER_H__
