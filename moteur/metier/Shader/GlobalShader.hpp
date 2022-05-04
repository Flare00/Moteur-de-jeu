#ifndef __GLOBAL_SHADER_H__
#define __GLOBAL_SHADER_H__

#define MAX_LIGHT 128
#include <string>
#include <vector>

#include "Shader.hpp"
#include "Transformation.hpp"
#include "Texture.hpp"
#include <Global.hpp>
#include <Light/Light.hpp>


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

	//Light number
	GLuint u_light_number;

	//Gamma correction
	GLuint u_gamma_correction;

	U_Material u_material;
	U_Light u_lights[MAX_LIGHT];
	std::vector<Light> lights;

public:
	GlobalShader(std::string vertex, std::string fragment) : Shader(vertex, fragment) {
		this->u_model = glGetUniformLocation(this->id, "u_model");
		this->u_texture = glGetUniformLocation(this->id, "u_texture");
		this->u_has_texture = glGetUniformLocation(this->id, "u_has_texture");

		this->u_material.ambiant = glGetUniformLocation(this->id, "u_material.ambiant");
		this->u_material.diffuse = glGetUniformLocation(this->id, "u_material.diffuse");
		this->u_material.specular = glGetUniformLocation(this->id, "u_material.specular");
		this->u_material.shininess = glGetUniformLocation(this->id, "u_material.shininess");

		this->u_gamma_correction = glGetUniformLocation(this->id, "u_gamma_correction");

		this->u_light_number = glGetUniformLocation(this->id, "u_light_number");
		for (int i = 0; i < MAX_LIGHT; i++) {
			this->u_lights[i].position = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].position")).c_str());
			this->u_lights[i].color = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].color")).c_str());
			this->u_lights[i].intensity = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].intensity")).c_str());
		}

		lights.push_back(Light(vec3(0, 1, -2), vec3(1, 1, 1), 3.0f));
	}

	void setLight() {
		int nbLight = (lights.size() < MAX_LIGHT ? lights.size() : MAX_LIGHT);
		glUniform1f(this->u_gamma_correction, 2.2f);
		glUniform1i(this->u_light_number, nbLight);
		//Load Lights
		for (int i = 0; i < nbLight; i++) {
			glUniform3fv(u_lights[i].position, 1, &(lights[i].getPosition())[0]);
			glUniform3fv(u_lights[i].color, 1, &(lights[i].getColor())[0]);
			glUniform1f(u_lights[i].intensity, lights[i].getIntensity());
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

		if (global_wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Draw
		glDrawElements(GL_TRIANGLES, size_indice, GL_UNSIGNED_INT, 0);
	}
};


#endif // !__GLOBAL_SHADER_H__
