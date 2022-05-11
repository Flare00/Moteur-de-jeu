#ifndef __DEBUG_DRAWER_HPP__
#define __DEBUG_DRAWER_HPP__

#include <LinearMath/btIDebugDraw.h>
#include <Shader/DebugShader.hpp>
#include <iostream>

class DebugDrawer : public btIDebugDraw
{
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	int debugMode;
	DebugShader* shader;

	std::vector<glm::vec3> points;
	std::vector<unsigned int> indices;
	unsigned int lastIndices = 0;

public:
	DebugDrawer(DebugShader* shader)
	{
		this->shader = shader;
		this->debugMode = 0;

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VAO);
		glGenBuffers(1, &this->EBO);
	}

	virtual void drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color, btScalar alpha)
	{
		this->points.push_back(glm::vec3(v0.getX(), v0.getY(), v0.getZ()));
		this->points.push_back(glm::vec3(v1.getX(), v1.getY(), v1.getZ()));
		this->points.push_back(glm::vec3(v2.getX(), v2.getY(), v2.getZ()));
		this->indices.push_back(lastIndices);
		this->indices.push_back(lastIndices + 1);
		this->indices.push_back(lastIndices + 2);
		this->lastIndices += 3;
	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
	}

	virtual void clearLines()
	{
		points.clear();
		indices.clear();
		lastIndices = 0;
	}

	virtual void flushLines()
	{
		if (this->points.size() > 0) {
			glBindVertexArray(VAO);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(vec3), &this->points[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

			this->shader->drawMesh(VAO, this->indices.size(), glm::vec3(1, 0, 0));
		}
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
		printf("%s\n", warningString);
	}

	virtual void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
	}

	virtual int getDebugMode() const
	{
		return debugMode;
	}
};

#endif
