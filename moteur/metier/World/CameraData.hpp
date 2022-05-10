#ifndef __CAMERA_DATA_HPP__
#define __CAMERA_DATA_HPP__

#include <glm/glm.hpp>
#include <World/Frustum.hpp>
#include <Shader/ShadowShader.hpp>

class CameraData
{
public:
	enum Type
	{
		CAMERA,
		SHADOW
	};

protected:
	glm::mat4 perspective;
	glm::mat4 view;
	glm::vec3 position;
	Frustum *frustum;
	Type type;
	ShadowShader *shadowShader = NULL;

public:
	CameraData(Type type, Frustum *frustum, glm::vec3 position = glm::vec3(0.0f), glm::mat4 perspective = glm::mat4(1.0f), glm::mat4 view = glm::mat4(1.0f))
	{
		this->position = position;
		this->perspective = perspective;
		this->view = view;
		this->type = type;
		this->frustum = frustum;
	}

	void setValues(glm::vec3 position, glm::mat4 perspective, glm::mat4 view)
	{
		this->position = position;
		this->perspective = perspective;
		this->view = view;
	}

	glm::mat4 getProjection()
	{
		return this->perspective;
	}

	glm::mat4 getView()
	{
		return this->view;
	}

	glm::vec3 getPosition()
	{
		return this->position;
	}

	Frustum *getFrustum()
	{
		return this->frustum;
	}

	void setPerspective(glm::mat4 a)
	{
		this->perspective = a;
	}
	void setView(glm::mat4 a)
	{
		this->view = a;
	}

	void setPosition(glm::vec3 a)
	{
		this->position = a;
	}

	void setFrustum(Frustum *frustum)
	{
		this->frustum = frustum;
	}

	float distanceFrom(glm::vec3 point)
	{
		return glm::distance(this->position, point);
	}

	Type getType()
	{
		return this->type;
	}

	ShadowShader *getShadowShader()
	{
		return this->shadowShader;
	}

	void setShadowShader(ShadowShader *shader)
	{
		this->shadowShader = shader;
	}
};

#endif // !__CAMERA_DATA_HPP__
