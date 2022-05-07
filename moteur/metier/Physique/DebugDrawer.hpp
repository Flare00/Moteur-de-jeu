#ifndef __DEBUG_DRAWER_HPP__
#define __DEBUG_DRAWER_HPP__

#include <LinearMath/btIDebugDraw.h>
#include <Shader/DebugShader.hpp>
#include <iostream>

class DebugDrawer : public btIDebugDraw {
private:
	GLuint VAO;
	GLuint VBO;
	int debugMode;
	DebugShader* shader;
public:
	DebugDrawer(DebugShader* shader)
	{
		this->shader = shader;
		this->debugMode = 0;


		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		glm::vec3 vertexArray[2];

		vertexArray[0] = glm::vec3(from.getX(), from.getY(), from.getZ());
		vertexArray[1] = glm::vec3(to.getX(), to.getY(), to.getZ());

		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), &vertexArray[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		this->shader->drawMesh(VAO, 2, glm::vec3(1, 0, 0));
	}

	virtual void setDebugMode(int debugMode)
	{
		this->debugMode = debugMode;
	}

	virtual void draw3dText(const btVector3& location, const char* textString)
	{
	}

	virtual void reportErrorWarning(const char* warningString)
	{
		printf(warningString);
	}

	virtual void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
	}
	virtual int getDebugMode() const {
		return debugMode;
	}
};

#endif
