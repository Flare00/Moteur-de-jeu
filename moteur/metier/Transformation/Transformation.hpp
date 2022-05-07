#ifndef __H_TRANSFORMATION__
#define __H_TRANSFORMATION__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Transformation/ITransformation.hpp>
class Transformation : public ITransformation
{
protected:

	// Transformation that can be applied to descendance.
	float scale;
	glm::vec3 rotation;
	glm::vec3 translation;

	// Self Transformation
	float selfScale;
	glm::vec3 selfRotation;
	glm::vec3 selfTranslation;

	glm::mat4 globalMatrix;
	glm::mat4 localMatrix;
	bool globalHasBeenChanged;
	bool localHasBeenChanged;


public:

	Transformation(float scale = 1.0f, glm::vec3 translate = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), float selfScale = 1.0f, glm::vec3 selfRotation = glm::vec3(0.0f), glm::vec3 selfTranslation = glm::vec3(0.0f))
	{
		this->scale = scale;
		this->rotation = rotation;
		this->translation = translate;
		this->selfScale = selfScale;
		this->selfRotation = selfRotation;
		this->selfTranslation = selfTranslation;
		this->globalMatrix = glm::mat4(1.0f);
		this->localMatrix = glm::mat4(1.0f);
		this->globalHasBeenChanged = true;
		this->localHasBeenChanged = true;
	}

	virtual ITransformation* translate(glm::vec3 t)
	{
		this->translation += t;
		this->globalHasBeenChanged = true;
		return this;
	}

	Transformation *selfTranslate(glm::vec3 t)
	{
		this->selfTranslation += t;
		this->localHasBeenChanged = true;
		return this;
	}

	virtual ITransformation*setTranslate(glm::vec3 pos)
	{
		this->translation = pos;
		this->globalHasBeenChanged = true;
		return this;
	}
	Transformation *setSelfTranslate(glm::vec3 pos)
	{
		this->selfTranslation = pos;
		this->localHasBeenChanged = true;
		return this;
	}

	Transformation *setScale(float s)
	{
		this->scale = s;
		this->globalHasBeenChanged = true;
		return this;
	}
	Transformation *setSelfScale(float s)
	{
		this->selfScale = s;
		this->localHasBeenChanged = true;
		return this;
	}

	virtual ITransformation*rotate(glm::vec3 v)
	{
		this->rotation = this->rotation + v;
		this->globalHasBeenChanged = true;
		return this;
	}

	Transformation *selfRotate(glm::vec3 v, float rad)
	{
		this->selfRotation = this->selfRotation + (v * rad);
		this->localHasBeenChanged = true;
		return this;
	}

	virtual ITransformation*setRotation(glm::vec3 r)
	{
		this->rotation = r;
		this->globalHasBeenChanged = true;
		return this;
	}
	Transformation *setSelfRotation(glm::vec3 r)
	{
		this->selfRotation = r;
		this->localHasBeenChanged = true;
		return this;
	}

	float getScale()
	{
		return this->scale;
	}
	float getSelfScale()
	{
		return this->selfScale;
	}
	virtual glm::vec3 getRotation()
	{
		return this->rotation;
	}
	glm::vec3 getSelfRotation()
	{
		return this->selfRotation;
	}
	virtual glm::vec3 getTranslation()
	{
		return this->translation;
	}
	glm::vec3 getSelfTranslation()
	{
		return this->selfTranslation;
	}

	void computeGlobalMatrix()
	{
		if (this->globalHasBeenChanged)
		{

			// Compute the Global Matrix only if it has been changed
			this->globalMatrix = glm::mat4(1.0f);

			this->globalMatrix = glm::scale(this->globalMatrix, glm::vec3(this->scale));

			this->globalMatrix = glm::rotate(this->globalMatrix, this->rotation[1], vec3(0, 1, 0));
			this->globalMatrix = glm::rotate(this->globalMatrix, this->rotation[0], vec3(1, 0, 0));
			this->globalMatrix = glm::rotate(this->globalMatrix, this->rotation[2], vec3(0, 0, 1));

			this->globalMatrix = glm::translate(this->globalMatrix, this->translation);

			this->globalHasBeenChanged = false;
		}
	}

	void computeLocalMatrix()
	{
		if (this->localHasBeenChanged)
		{
			// Compute the Local Matrix only if it has been changed
			this->localMatrix = glm::mat4(1.0f);

			this->localMatrix = glm::scale(this->localMatrix, glm::vec3(this->selfScale));

			this->localMatrix = glm::rotate(this->localMatrix, this->selfRotation[1], vec3(0, 1, 0));
			this->localMatrix = glm::rotate(this->localMatrix, this->selfRotation[0], vec3(1, 0, 0));
			this->localMatrix = glm::rotate(this->localMatrix, this->selfRotation[2], vec3(0, 0, 1));

			this->localMatrix = glm::translate(this->localMatrix, this->selfTranslation);

			this->localHasBeenChanged = false;
		}
	}

	virtual glm::mat4 getGlobalMatrix(glm::mat4 globalParent = glm::mat4(1.0f))
	{
		computeGlobalMatrix();
		return globalParent * this->globalMatrix;
	}

	virtual glm::mat4 getMatrix(glm::mat4 globalParent = glm::mat4(1.0f))
	{
		computeGlobalMatrix();
		computeLocalMatrix();
		return globalParent * this->globalMatrix * this->localMatrix;
	}
};

#endif