#ifndef __INPUT_H__
#define __INPUT_H__
#include <stdio.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <Global.hpp>

#include <Callback/IGlobalGameCallback.hpp>
#include <Callback/IGlobalSceneCallback.hpp>

class Input
{
protected:
	IGlobalGameCallback *gameCallback = NULL;
	IGlobalSceneCallback *sceneCallback = NULL;
	bool pausePress = false;
	bool nextScenePress = false;
	bool nextCameraPress = false;

public:
	bool pause = false;

	Input(IGlobalGameCallback *gameCallback, IGlobalSceneCallback *sceneCallback)
	{
		this->gameCallback = gameCallback;
		this->sceneCallback = sceneCallback;
	}

	virtual void mouseInput() = 0;
	virtual void keyboardInput(float deltaTime)
	{

		if (glfwGetKey(global_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(global_window, true);
		// Permet de changer la valeur de l'attribut "pause" permettant d'avoir soit une texture_low, soit une coloration selon la position.
		if (glfwGetKey(global_window, GLFW_KEY_P) == GLFW_PRESS && !pausePress)
		{
			pause = !pause;
			pausePress = true;
		}
		// Permet de ne pas changer la valeur a chaque frame mais d'attendre que le bouton soit relacher.
		else if (glfwGetKey(global_window, GLFW_KEY_P) == GLFW_RELEASE && pausePress)
		{
			pausePress = false;
		}

		if (glfwGetKey(global_window, GLFW_KEY_ENTER) == GLFW_PRESS && !nextScenePress)
		{
			if (gameCallback != NULL)
			{
				gameCallback->askNextScene(true);
			}
			nextScenePress = true;
		}
		else if (glfwGetKey(global_window, GLFW_KEY_ENTER) == GLFW_RELEASE && nextScenePress)
		{
			nextScenePress = false;
		}

		if (glfwGetKey(global_window, GLFW_KEY_TAB) == GLFW_PRESS && !nextCameraPress)
		{
			sceneCallback->askNextCamera(true);
			nextCameraPress = true;
		}
		else if (glfwGetKey(global_window, GLFW_KEY_TAB) == GLFW_RELEASE && nextCameraPress)
		{
			nextCameraPress = false;
		}
	}
	virtual void processInput(float deltaTime)
	{
		mouseInput();
		keyboardInput(deltaTime);
	}
};

#endif