#ifndef __INPUT_COLLISION_H__
#define __INPUT_COLLISION_H__
#include <stdio.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <GameObject/GameObject.hpp>
#include "Camera.hpp"

using namespace glm;

class InputCollision {
public:
	bool pause = false;
	bool rotateTerrain = false;
	float rotateSpeed = 1.0f;

private:
	GameObject* player;
	Camera* camera;

	bool cameraPosPress = false;
	bool pausePress = false;
	bool cameraModePress = false;
	bool terrainRotatePress = false;

	//Mouse movement
	int firstMouseMovement = 2;
	float lastMouseX, lastMouseY;
	float sensivityMouseX = 0.001f;
	float sensivityMouseY = 0.001f;

	//Process Mouse input
	void mouseInput(GLFWwindow* window) {
		GLdouble xPos;
		GLdouble yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		if (this->firstMouseMovement > 0) {
			firstMouseMovement--;
			lastMouseX = (float)xPos;
			lastMouseY = (float)yPos;
		}
		else {
			float xOffset = (float)xPos - lastMouseX;
			float yOffset = lastMouseY - (float)yPos;

			lastMouseX = (float)xPos;
			lastMouseY = (float)yPos;

			this->camera->rotate(CameraAxe::X, yOffset * sensivityMouseX);
			this->camera->rotate(CameraAxe::Y, xOffset * sensivityMouseY);
		}
		//void rotate(CameraAxe axe, bool sensPositif, float deltaTime) {
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void keyboardInput(GLFWwindow* window, float deltaTime)
	{
		//Permet de changer la valeur de l'attribut "pause" permettant d'avoir soit une texture_low, soit une coloration selon la position.
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pausePress) {
			pause = !pause;
			pausePress = true;
		}
		//Permet de ne pas changer la valeur a chaque frame mais d'attendre que le bouton soit relacher.
		else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE && pausePress) {
			pausePress = false;
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (!pause) {
			//Mode de camera
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cameraModePress) {
				camera->switchOrbitalMode();
				cameraModePress = true;
			}
			else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && cameraModePress) {
				cameraModePress = false;
			}

			//Permet de changer la valeur de l'attribut "isTexture" permettant d'avoir soit une texture_low, soit une coloration selon la position.
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !cameraPosPress) {
				std::cout << "Camera Pos   : [ " << camera->position.x << " ; " << camera->position.y << " ; " << camera->position.z << " ]" << std::endl;
				std::cout << "Camera Rotat : [ " << camera->yaw << " ; " << camera->pitch << " ; " << camera->roll << " ]" << std::endl;
				std::cout << "Camera Front : [ " << camera->front.x << " ; " << camera->front.y << " ; " << camera->front.z << " ]" << std::endl;
				cameraPosPress = true;
			}
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE && cameraPosPress) {
				cameraPosPress = false;
			}


			//Rotation de la camera
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				camera->move(CameraAxe::X, false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				camera->move(CameraAxe::X, true, deltaTime);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				camera->move(CameraAxe::Z, true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
				camera->move(CameraAxe::Z, false, deltaTime);


			//Rotation de la camera
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				player->getTransform()->translate(glm::vec3(deltaTime*5,0,0));
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				player->getTransform()->translate(glm::vec3(-deltaTime*5, 0, 0));


			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				player->getTransform()->translate(glm::vec3(0, 0, deltaTime*5));

			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				player->getTransform()->translate(glm::vec3(0, 0, -deltaTime*5));



			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				camera->move(CameraAxe::Y, true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				camera->move(CameraAxe::Y, false, deltaTime);


			/*//Pr�cision
			if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
				meshPrecision++;
				//generate_plane((Modele*)scene->getChild(0), meshPrecision, meshPrecision, meshSize, meshSize);
				std::cout << "Precision : " << meshPrecision << std::endl;
			}

			if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
				meshPrecision--;
				//generate_plane((Modele*)scene->getChild(0), meshPrecision, meshPrecision, meshSize, meshSize);
				std::cout << "Precision : " << meshPrecision << std::endl;
			}*/


			//Rotation de terrain
			if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
				rotateSpeed += 0.05f;
			if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
				rotateSpeed -= 0.05f;
			;


			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !terrainRotatePress) {
				rotateTerrain = !rotateTerrain;
				terrainRotatePress = true;
			}
			else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && terrainRotatePress) {
				terrainRotatePress = false;
			}
		}
	}

public:

	InputCollision(Camera* camera, GameObject* player) {
		this->camera = camera;
		this->player = player;
	}

	void processInput(GLFWwindow* window, float deltaTime)
	{
		mouseInput(window);
		keyboardInput(window, deltaTime);
	}

	void SetCamera(Camera* camera) {
		this->camera = camera;
	}

};



#endif