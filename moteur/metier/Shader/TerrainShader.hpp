#ifndef __TERRAIN_SHADER_H__
#define __TERRAIN_SHADER_H__

#include "GlobalShader.hpp"
#include "Texture.hpp"

class TerrainShader : public GlobalShader {
protected:
	//Height Map
	GLuint u_heightmap;
	GLuint u_has_heightmap;

	//Textures
	GLuint u_texture_top;
	GLuint u_texture_bottom;

	GLuint u_limit_top;
	GLuint u_limit_bottom;
	GLuint u_limit_blur;
public:
	TerrainShader(std::string vertex, std::string fragment) : GlobalShader(vertex, fragment) {
		this->use();

		this->u_heightmap = glGetUniformLocation(this->id, "u_heightmap");
		this->u_has_heightmap = glGetUniformLocation(this->id, "u_has_heightmap");

		this->u_texture_top = glGetUniformLocation(this->id, "u_texture_top");
		this->u_texture_bottom = glGetUniformLocation(this->id, "u_texture_bottom");

		this->u_limit_top = glGetUniformLocation(this->id, "u_limit_top");
		this->u_limit_bottom = glGetUniformLocation(this->id, "u_limit_bottom");
		this->u_limit_blur = glGetUniformLocation(this->id, "u_limit_blur");
	}

	void drawHeightMap(Texture* texture, int id) {
		if (texture != NULL) {
			texture->draw( this->u_heightmap, id);
			glUniform1i(this->u_has_heightmap, 1);
		}
		else {
			glUniform1i(this->u_has_heightmap, 0);
		}
	}

	void drawTextureTop(Texture* texture, int id) {
		if (texture != NULL) {
			texture->draw(this->u_texture_top, id);
		}
	}

	void drawTextureBottom(Texture* texture, int id) {
		if (texture != NULL) {
			texture->draw(this->u_texture_bottom, id);
		}
	}

	void setLimit(float limitTop, float limitBottom, float limitBlur) {
		glUniform1f(this->u_limit_top, limitTop);
		glUniform1f(this->u_limit_bottom, limitBottom);
		glUniform1f(this->u_limit_blur, limitBlur);
	}
	
};


#endif // !__GLOBAL_SHADER_H__
