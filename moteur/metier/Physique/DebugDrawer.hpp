#ifndef __DEBUG_DRAWER_HPP__
#define __DEBUG_DRAWER_HPP__

/*
#include <LinearMath/btIDebugDraw.h>
#include <Shader/DebugShader.hpp>

class DebugDrawer : public btIDebugDraw {
private:
	int debugMode;
	DebugShader* shader;
public:
	DebugDrawer()
	{
		this->shader = new DebugShader("Shaders/Debug/debug_vertex.glsl", "Shaders/Debug/debug_fragment.glsl");
		this->debugMode = 0;
	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		float tmp[6] = { from.getX(), from.getY(), from.getZ(),
						to.getX(), to.getY(), to.getZ() };
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1,&VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 6, &tmp, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);

		this->shader->drawMesh(VAO, 6, glm::mat4(1.0f), glm::vec3(1, 0, 0));

		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
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
};*/

#endif
