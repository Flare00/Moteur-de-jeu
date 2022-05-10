#ifndef __SHADOW_MAP_HPP__
#define __SHADOW_MAP_HPP__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Shader/ShadowShader.hpp>
#include <World/CameraData.hpp>
#include <GameObject/GameObject.hpp>

class ShadowMap
{
public:
protected:
	ShadowShader *shader;
	GLuint depthMapFBO;
	GLuint depthMapTexture;

	unsigned int depthMapSize;

	bool valide = true;

	CameraData data;

	float size = 10.0f, aspect = 1.0f, zNear = 1.0f, zFar = 100.0f;

public:
	/// <summary>
	/// Resolution de 1 � 12, (calcul 2^n, si Resolution = 4 alors shadow map de taille 16, si Resolution = 12 alors shadow map de taille 4096)
	/// </summary>
	ShadowMap(int resolution = 10, float portee = 100.0f) : data(CameraData::SHADOW, new Frustum())
	{
		zFar = portee;
		this->shader = new ShadowShader("Shaders/Shadow/shadow_vertex.glsl", "Shaders/Shadow/shadow_fragment.glsl"); //,"", "Shaders/Shadow/shadow_tessControl.glsl","Shaders/Shadow/shadow_tessEval.glsl");
		this->depthMapSize = (unsigned int)glm::pow(2, resolution);
		data.setShadowShader(this->shader);
		// Alloue le framebuffer
		glGenFramebuffers(1, &this->depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);

		// Alloue la texture Shadowmap
		glGenTextures(1, &depthMapTexture);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, this->depthMapSize, this->depthMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// Set la texture sur le framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthMapTexture, 0);
		glDrawBuffer(GL_NONE); // No color buffer is drawn to.
		// glReadBuffer(GL_NONE);

		// Verifie que le framebufffer est bon
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			this->valide = true;
		}
		else
		{
			std::cout << "PROBLEM IN FBO ShadowMap : FBO NOT successfully created" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			this->valide = false;
		}
	}

	~ShadowMap()
	{
		glDeleteFramebuffers(1, &this->depthMapFBO);
	}

	void drawData()
	{
		if (this->valide)
		{
			this->bind();
			this->shader->drawView(this->getLightMatrix());
		}
	}

	void bind()
	{
		if (valide)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
			glViewport(0, 0, depthMapSize, depthMapSize);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	bool isValide()
	{
		return this->valide;
	}

	void setMappingParameter(glm::vec3 position, glm::vec3 direction)
	{
		data.setValues(position,
					   glm::perspective<float>(glm::radians(45.0f), this->aspect, this->zNear, this->zFar),
					   glm::lookAt(position, position - direction, glm::vec3(0, -1, 0)));
		// data.setValues(position, glm::ortho(-size, size,-size, size, this->zNear, this->zFar), glm::lookAt(position,/*position + direction*/ glm::vec3(0), glm::vec3(0, 1, 0)));
		data.getFrustum()->init(data.getProjection(), data.getView(), size, this->zNear, this->zFar);
	}

	void showMinMax()
	{
		glNamedFramebufferReadBuffer(depthMapFBO, GL_COLOR_ATTACHMENT0);
		float *pixels = new float[depthMapSize * depthMapSize];
		glReadPixels(0, 0, depthMapSize, depthMapSize, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);

		float min = FLT_MAX, max = -FLT_MAX;
		unsigned int k = 0;
		for (unsigned int i = 0; i < depthMapSize; i++)
		{
			for (unsigned int j = 0; j < depthMapSize; j++)
			{
				if (min > pixels[k])
					min = pixels[k];
				if (max < pixels[k])
					max = pixels[k];
				k++;
			}
		}

		printf("Min : %f | Max : %f\n", min, max);

		delete pixels;
	}

	void save_PPM_file(std::string const &filename)
	{

		std::ofstream output_image(filename.c_str());

		/// READ THE CONTENT FROM THE FBO
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		float *pixels = new float[depthMapSize * depthMapSize];
		glReadPixels(0, 0, depthMapSize, depthMapSize, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);

		output_image << "P3" << std::endl;
		output_image << depthMapSize << " " << depthMapSize << std::endl;
		// output_image << "255" << std::endl;
		output_image << "255" << std::endl;

		int k = 0;

		for (unsigned int i = 0; i < depthMapSize; i++)
		{
			for (unsigned int j = 0; j < depthMapSize; j++)
			{
				unsigned int val = (unsigned int)(255.0f * pixels[k]);
				output_image << val << " " << val << " " << val << " ";
				k = k + 1;
			}
			output_image << std::endl;
		}
		delete pixels;
		output_image.close();
		printf("Write : %s\n", filename.c_str());
	}

	static void bindScreen()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	CameraData *getData()
	{
		return &this->data;
	}

	glm::mat4 getLightMatrix()
	{
		return this->data.getProjection() * this->data.getView();
	}

	GLuint getDepthTexture()
	{
		return this->depthMapTexture;
	}
};

#endif // !__SHADOW_MAP_HPP__
