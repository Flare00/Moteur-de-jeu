#ifndef __INPUT_LIGHT_H__
#define __INPUT_LIGHT_H__
#include <stdio.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <Global.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <GameObject/GameObject.hpp>
#include <Global.hpp>
#include "Camera.hpp"
#include "Input.hpp"
#include <Callback/ILightCallback.hpp>
using namespace glm;

class InputLight : public Input
{
protected:
	ILightCallback *lightCallback = NULL;

	bool wireframeModePress = false;
	bool torchModePress = false;
	bool physicDebugModePress = false;
	// Mouse movement
	int firstMouseMovement = 2;
	float lastMouseX, lastMouseY;
	float sensivityMouseX = 0.001f;
	float sensivityMouseY = 0.001f;

	// Process Mouse input
	virtual void mouseInput()
	{
		GLdouble xPos;
		GLdouble yPos;
		glfwGetCursorPos(global_window, &xPos, &yPos);
		// std::cout << "xPos : " << xPos << " | yPos : " << yPos << std::endl;

		if (this->firstMouseMovement > 0)
		{
			firstMouseMovement--;
			lastMouseX = (float)xPos;
			lastMouseY = (float)yPos;
		}
		else
		{
			float xOffset = (float)xPos - lastMouseX;
			float yOffset = lastMouseY - (float)yPos;

			lastMouseX = (float)xPos;
			lastMouseY = (float)yPos;

			lightCallback->mouseMovement(yOffset * sensivityMouseX, xOffset * sensivityMouseY);
		}
		// void rotate(CameraAxe axe, bool sensPositif, float deltaTime) {
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	virtual void keyboardInput(float deltaTime)
	{
		Input::keyboardInput(deltaTime);

		if (!pause)
		{
			if (glfwGetKey(global_window, GLFW_KEY_A) == GLFW_PRESS)
				this->lightCallback->moveDirection(0, ILightCallback::GAUCHE, deltaTime);
			if (glfwGetKey(global_window, GLFW_KEY_D) == GLFW_PRESS)
				this->lightCallback->moveDirection(0, ILightCallback::DROITE, deltaTime);
			if (glfwGetKey(global_window, GLFW_KEY_W) == GLFW_PRESS)
				this->lightCallback->moveDirection(0, ILightCallback::DEVANT, deltaTime);
			if (glfwGetKey(global_window, GLFW_KEY_S) == GLFW_PRESS)
				this->lightCallback->moveDirection(0, ILightCallback::DERRIERE, deltaTime);
			if (glfwGetKey(global_window, GLFW_KEY_SPACE) == GLFW_PRESS)
				this->lightCallback->moveDirection(0, ILightCallback::HAUT, deltaTime);
			if (glfwGetKey(global_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				this->lightCallback->moveDirection(0, ILightCallback::BAS, deltaTime);

			if (glfwGetKey(global_window, GLFW_KEY_T) == GLFW_PRESS && !torchModePress)
			{
				lightCallback->toggleTorchMode();
				torchModePress = true;
			}
			else if (glfwGetKey(global_window, GLFW_KEY_T) == GLFW_RELEASE && torchModePress)
			{
				torchModePress = false;
			}

			if (glfwGetKey(global_window, GLFW_KEY_G) == GLFW_PRESS && !physicDebugModePress)
			{
				lightCallback->togglePhysicDebug();
				physicDebugModePress = true;
			}
			else if (glfwGetKey(global_window, GLFW_KEY_G) == GLFW_RELEASE && physicDebugModePress)
			{
				physicDebugModePress = false;
			}
		}
	}

public:
	InputLight(IGlobalGameCallback *gameCallback, IGlobalSceneCallback *sceneCallback, ILightCallback *lightCallback) : Input(gameCallback, sceneCallback)
	{
		this->lightCallback = lightCallback;
	}
};

#endif