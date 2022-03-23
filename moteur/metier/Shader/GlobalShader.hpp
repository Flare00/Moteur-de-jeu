#ifndef __GLOBAL_SHADER_H__
#define __GLOBAL_SHADER_H__

#define NB_LIGHT 1
#include <string>
#include "Shader.hpp"
#include "Transformation.hpp"
#include "Texture.hpp"
class GlobalShader : public Shader {
protected:
	struct U_Material {
		GLuint ambiant;
		GLuint diffuse;
		GLuint specular;
		GLuint shininess;
	};

	struct U_Light {
		GLuint position;
		GLuint color;
		GLuint intensity;
	};

	//vertex
	GLuint u_model;

	//fragment
	GLuint u_texture;
	GLuint u_has_texture;

	U_Material u_material;
	U_Light u_lights[NB_LIGHT];

public:
	GlobalShader(std::string vertex, std::string fragment) : Shader(vertex, fragment) {
		this->u_model = glGetUniformLocation(this->id, "u_model");
		this->u_texture = glGetUniformLocation(this->id, "u_texture");
		this->u_has_texture = glGetUniformLocation(this->id, "u_has_texture");

		this->u_material.ambiant = glGetUniformLocation(this->id, "u_material.ambiant");
		this->u_material.diffuse = glGetUniformLocation(this->id, "u_material.diffuse");
		this->u_material.specular = glGetUniformLocation(this->id, "u_material.specular");
		this->u_material.shininess = glGetUniformLocation(this->id, "u_material.shininess");

		for (int i = 0; i < NB_LIGHT; i++) {
			std::string lightID = std::string("u_lights[") + std::to_string(i) + std::string("]");
			this->u_lights[i].position = glGetUniformLocation(this->id, (lightID + ".position").c_str());
			this->u_lights[i].color = glGetUniformLocation(this->id, (lightID + ".color").c_str());
			this->u_lights[i].position = glGetUniformLocation(this->id, (lightID + ".intensity").c_str());
		}
	}

	void setLightTest() {
		this->use();
		for (int i = 0; i < 1; i++) {
			glUniform3fv(this->u_lights[i].position, 1, &vec3(0, 5, 0)[0]);
			glUniform3fv(this->u_lights[i].color, 1, &vec3(255, 255, 255)[0]);
			glUniform1f(this->u_lights[i].intensity, 150.0f);
		}
	}

	void drawMaterial(Material) {

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

	void drawMesh(GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix, Material material) {
		//Load Vertex
		glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);
		glUniform3fv(this->u_material.ambiant, 1, &material.getAmbiant()[0]);
		glUniform3fv(this->u_material.diffuse, 1, &material.getDiffuse()[0]);
		glUniform3fv(this->u_material.specular, 1, &material.getSpecular()[0]);
		glUniform1f(this->u_material.shininess,  material.getShininess());
		// Bind VAO
		glBindVertexArray(VAO);

		// Draw
		glDrawElements(GL_TRIANGLES, size_indice, GL_UNSIGNED_INT, 0);
	}
};


#endif // !__GLOBAL_SHADER_H__
