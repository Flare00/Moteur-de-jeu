#ifndef __INPUT_COLLISION_V2_HPP__
#define __INPUT_COLLISION_V2_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <GameObject/GameObject.hpp>
#include <World/Camera.hpp>
#include "Input.hpp"

class InputCollisionV2 : public Input {
private :
    Camera * camera = NULL;
    GameObject * player = NULL;
    int firstMouseMovement = 2;
    float lastMouseX, lastMouseY;
	float sensivityMouseX = 0.001f;
	float sensivityMouseY = 0.001f;
public :
    InputCollisionV2(Camera * camera, GameObject* player) : Input(){
        this->camera = camera;
        this->player = player;
    }

	virtual void processInput(float deltaTime)
	{
        key_input();
        cursor_input();
	}

    void key_input()
	{
		if (glfwGetKey(global_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			global_stop = true;
		}
	}
    void cursor_input()
	{
        GLdouble xPos, yPos;
		glfwGetCursorPos(global_window, &xPos, &yPos);
		std::cout << "xPos : " << xPos << " | yPos : " << yPos << std::endl;

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
	}
};


#endif