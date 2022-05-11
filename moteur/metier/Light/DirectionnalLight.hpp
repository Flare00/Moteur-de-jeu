#ifndef __DIRECTIONNAL_LIGHT_HPP__
#define __DIRECTIONNAL_LIGHT_HPP__
#include <glm/glm.hpp>
#include <Light/ILight.hpp>

#include <GameObject/GameObject.hpp>
using namespace glm;

class DirectionnalLight : public ILight
{
protected:
	ShadowMap shadow;
	glm::vec3 direction;
	bool generateShadow;
	bool first = true;

public:
	/// <summary>
	/// shadowResolution de 1 � 12, (calcul 2^n, si shadowResolution = 4 alors shadow map de taille 16, si shadowResolution = 12 alors shadow map de taille 4096)
	/// </summary>
	DirectionnalLight(vec3 position, vec3 direction, vec3 color = vec3(1), float intensity = 1.0f, float distance = 500.0f, int shadowResolution = 8, bool generateShadow = true) : ILight(Type::DIRECTIONAL, position, color, intensity), shadow(shadowResolution)
	{
		this->direction = normalize(direction);
		this->shadow.setMappingParameter(this->position, direction); // this->position - this->direction);
		this->generateShadow = generateShadow;
	}

	virtual void compute(GameObject *scene)
	{
		if (generateShadow)
		{
			if (this->dirty)
			{
				this->shadow.setMappingParameter(this->position, this->direction);
				this->dirty = false;
			}
			glCullFace(GL_FRONT);
			this->shadow.drawData();
			scene->compute(this->shadow.getData(), NULL, true);
			glCullFace(GL_BACK);
			/*if (first)
			{
				this->shadow.save_PPM_file("test.ppm");
				first = false;
			}*/
		}
	}

	vec3 getDirection()
	{
		return this->direction;
	}

	void setDirection(vec3 direction)
	{
		this->direction = normalize(direction);
		this->dirty = true;
	}

	ShadowMap *getShadowMap()
	{
		return &this->shadow;
	}

	bool isGeneratingShadow()
	{
		return this->generateShadow;
	}

	void setGenerateShadow(bool generate)
	{
		this->generateShadow = generate;
	}
};

#endif
