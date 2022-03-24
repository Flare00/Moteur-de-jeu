#ifndef __INPUT_H__
#define __INPUT_H__
#include <stdio.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <iostream>

class Input {
public:

	Input() {
		glfwSetInputMode(global_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	virtual void processInput(float deltaTime)
	{
	}

};



#endif