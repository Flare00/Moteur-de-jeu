#ifndef __SHADOW_MAP_HPP__
#define __SHADOW_MAP_HPP__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Shader/Shader.hpp>
class ShadowMap {
protected:

	Shader* shader;
	GLuint depthMapFBO;
	GLuint depthMapTexture;

	unsigned int depthMapSize;

	bool valide = true;;

public:
	/// <summary>
	/// Resolution de 1 à 12, (calcul 2^n, si Resolution = 4 alors shadow map de taille 16, si Resolution = 12 alors shadow map de taille 4096)
	/// </summary>
	ShadowMap(int resolution = 10) {
		this->depthMapSize = glm::pow(2,resolution);

		//Alloue le framebuffer
		glGenFramebuffers(1, &this->depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);

		//Alloue la texture Shadowmap
		glGenTextures(1, &depthMapTexture);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, this->depthMapSize, this->depthMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Set la texture sur le framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthMapTexture, 0);
		glDrawBuffer(GL_NONE); // No color buffer is drawn to.

		//Verifie que le framebufffer est bon
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else {
			std::cout << "PROBLEM IN FBO ShadowMap : FBO NOT successfully created" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			valide = false;
		}
	}

	~ShadowMap() {
		glDeleteFramebuffers(1, &this->depthMapFBO);
	}


	void bind() {
		if (valide) {
			glViewport(0, 0, depthMapSize, depthMapSize);
			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	bool isValide() {
		return this->valide;
	}

	void save_PPM_file(std::string const& filename) {
		std::ofstream output_image(filename.c_str());

		/// READ THE CONTENT FROM THE FBO
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		float* pixels = new float[depthMapSize * depthMapSize];
		glReadPixels(0, 0, depthMapSize, depthMapSize, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);

		output_image << "P3" << std::endl;
		output_image << depthMapSize << " " << depthMapSize << std::endl;
		//output_image << "255" << std::endl;
		output_image << "255" << std::endl;

		int k = 0;

		for (int i = 0; i < depthMapSize; i++) {
			for (int j = 0; j < depthMapSize; j++) {
				unsigned int val = 255 * pixels[k];
				output_image << val << " " << val << " " << val << " ";
				k = k + 1;
			}
			output_image << std::endl;
		}
		delete pixels;
		output_image.close();
	}

	static void bindScreen() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};

#endif // !__SHADOW_MAP_HPP__
