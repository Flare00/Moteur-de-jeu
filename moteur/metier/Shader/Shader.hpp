#ifndef __SHADER_H__
#define __SHADER_H__
#include <stdio.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include <common/shader.hpp>
#include <common/vboindexer.hpp>

#include "../World/Camera.hpp"


class Shader {
protected:
	GLuint id;
	std::string vertex;
	std::string fragment;

	GLuint u_view;
	GLuint u_projection;
	GLuint u_camera_transformation;

public:
	Shader(std::string vertex, std::string fragment) {
		this->id = LoadShaders(vertex.c_str(), fragment.c_str());
		this->vertex = vertex;
		this->fragment = fragment;

		glUseProgram(id);
		this->u_view = glGetUniformLocation(this->id, "u_view");
		this->u_projection = glGetUniformLocation(this->id, "u_projection");
		this->u_camera_transformation = glGetUniformLocation(this->id, "u_camera_transformation");
	}

	virtual ~Shader() {
		glDeleteProgram(id);
	}

	void use() {
		glUseProgram(id);
	}

	void drawView(Camera* camera) {
		glUseProgram(id);
		glUniformMatrix4fv(this->u_view, 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
		glUniformMatrix4fv(this->u_projection, 1, GL_FALSE, &camera->getProjection()[0][0]);
		glUniformMatrix4fv(this->u_camera_transformation, 1, GL_FALSE, &camera->getTransformationMatrix()[0][0]);
	}

	GLuint getProgramID() {
		return this->id;
	}
};


#endif
