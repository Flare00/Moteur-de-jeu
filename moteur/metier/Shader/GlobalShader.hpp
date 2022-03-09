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

	void drawMesh(GLuint vertexbuffer, GLuint texcoordbuffer, GLuint elementbuffer, std::vector<unsigned int>* indices, glm::mat4 transformMatrix) {
		//Load Vertex
		glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,  // stride
			(void*)0            // array buffer offset
		);




		//Loads UV
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
		glVertexAttribPointer(
			1,                  // attribute
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,  				// stride
			(void*)0            // array buffer offset
		);
		
		//Draw Triangles 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices->size(),    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
		);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
};


#endif // !__GLOBAL_SHADER_H__
