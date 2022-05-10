#ifndef __GLOBAL_SHADER_H__
#define __GLOBAL_SHADER_H__

#define MAX_LIGHT 28
#include <string>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"
#include <Global.hpp>
#include <Light/Material.hpp>
#include <Light/ILight.hpp>
#include <Light/DirectionnalLight.hpp>
#include <World/CameraData.hpp>

class GlobalShader : public Shader
{
protected:
	struct U_Material
	{
		GLuint ambiant;
		GLuint diffuse;
		GLuint specular;
		GLuint shininess;
	};

	struct U_Light
	{
		GLuint position;
		GLuint color;
		GLuint intensity;
		GLuint light_dir;
		GLuint generate_shadows;
		GLuint shadow_map;
		GLuint light_matrix;
	};

	// vertex
	GLuint u_model;

	// fragment
	std::vector<GLuint> u_textures;
	GLuint u_nb_texture;

	// Light number
	GLuint u_light_number;

	// Gamma correction
	GLuint u_gamma_correction;

	U_Material u_material;
	U_Light u_lights[MAX_LIGHT];
	float nbTexture = 0;

public:
	GlobalShader(std::string vertex, std::string fragment, std::string geometry = "", std::string tessControl = "", std::string tessEval = "") : Shader(vertex, fragment, geometry, tessControl, tessEval)
	{
		this->u_model = glGetUniformLocation(this->id, "u_model");

		for (int i = 0; i < 4; i++)
		{
			this->u_textures.push_back(glGetUniformLocation(this->id, (std::string("u_textures[") + std::to_string(i) + std::string("]")).c_str()));
		}
		this->u_nb_texture = glGetUniformLocation(this->id, "u_nb_texture");

		this->u_material.ambiant = glGetUniformLocation(this->id, "u_material.ambiant");
		this->u_material.diffuse = glGetUniformLocation(this->id, "u_material.diffuse");
		this->u_material.specular = glGetUniformLocation(this->id, "u_material.specular");
		this->u_material.shininess = glGetUniformLocation(this->id, "u_material.shininess");

		this->u_gamma_correction = glGetUniformLocation(this->id, "u_gamma_correction");

		this->u_light_number = glGetUniformLocation(this->id, "u_light_number");
		for (int i = 0; i < MAX_LIGHT; i++)
		{
			this->u_lights[i].position = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].position")).c_str());
			this->u_lights[i].color = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].color")).c_str());
			this->u_lights[i].intensity = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].intensity")).c_str());
			this->u_lights[i].generate_shadows = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].generate_shadows")).c_str());
			this->u_lights[i].shadow_map = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].shadow_map")).c_str());
			this->u_lights[i].light_dir = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].light_dir")).c_str());
			this->u_lights[i].light_matrix = glGetUniformLocation(this->id, (std::string("u_lights[") + std::to_string(i) + std::string("].light_matrix")).c_str());
		}
		// lights.push_back(Light(vec3(0, 1, -2), vec3(1, 1, 1), 3.0f));
	}

	void setLights(std::vector<ILight *> lights)
	{
		int nbLight = 0;
		glUniform1f(this->u_gamma_correction, gammaCorrection);
		// Load Lights
		for (size_t i = 0, max = lights.size(); i < max && nbLight < MAX_LIGHT; i++)
		{
			if (lights[i]->isActive())
			{

				glm::vec3 lightDir(0.0f);
				glm::mat4 lightMatrix(1.0f);
				bool generateShadow = false;
				glUniform3fv(u_lights[nbLight].position, 1, &(lights[i]->getPosition())[0]);
				glUniform3fv(u_lights[nbLight].color, 1, &(lights[i]->getColor())[0]);
				glUniform1f(u_lights[nbLight].intensity, lights[i]->getIntensity());
				// Passe les shadow
				if (lights[i]->getType() == ILight::DIRECTIONAL)
				{
					DirectionnalLight *tmpLight = (DirectionnalLight *)lights[i];
					lightDir = tmpLight->getDirection();
					if (tmpLight->isGeneratingShadow())
					{
						generateShadow = true;
						lightMatrix = tmpLight->getShadowMap()->getLightMatrix();
						// bind la texture,4 texture reserv� pour le mod�le
						glActiveTexture(GL_TEXTURE0 + this->nbTexture + nbLight);
						glBindTexture(GL_TEXTURE_2D, tmpLight->getShadowMap()->getDepthTexture());
						glUniform1i(this->u_lights[nbLight].shadow_map, this->nbTexture + nbLight);
					}
				}
				glUniformMatrix4fv(u_lights[nbLight].light_matrix, 1, GL_FALSE, &lightMatrix[0][0]);
				glUniform3fv(u_lights[nbLight].light_dir, lights[i]->getIntensity(), &lightDir[0]);
				glUniform1i(u_lights[nbLight].generate_shadows, (generateShadow ? 1 : 0));
				nbLight++;
			}
		}
		glUniform1i(this->u_light_number, nbLight);
	}

	void drawMaterial(Material material)
	{
		glUniform3fv(this->u_material.ambiant, 1, &material.getAmbiant()[0]);
		glUniform3fv(this->u_material.diffuse, 1, &material.getDiffuse()[0]);
		glUniform3fv(this->u_material.specular, 1, &material.getSpecular()[0]);
		glUniform1f(this->u_material.shininess, material.getShininess());
	}

	void drawTexture(Texture *texture, int id)
	{
		if (texture != NULL)
		{
			texture->draw(this->u_textures[id], id);
			this->nbTexture++;
		}
	}

	virtual void drawMesh(GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix)
	{
		glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);
		glUniform1i(this->u_nb_texture, (GLint)this->nbTexture);

		if (global_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Bind VAO

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, size_indice, GL_UNSIGNED_INT, 0);
	}

	void resetNbTexture()
	{
		this->nbTexture = 0;
	}
};

#endif // !__GLOBAL_SHADER_H__
