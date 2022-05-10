#ifndef __INPUT_PROJECTILE_H__
#define __INPUT_PROJECTILE_H__
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
#include <Callback/IProjectileCallback.hpp>

using namespace glm;

class InputProjectile : public Input
{

protected:
    IProjectileCallback *callback;

    bool wireframeModePress = false;

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
            callback->mouseMovement(yOffset * sensivityMouseX, xOffset * sensivityMouseY);

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
                this->callback->moveDirection(0, IProjectileCallback::GAUCHE, deltaTime);
            if (glfwGetKey(global_window, GLFW_KEY_D) == GLFW_PRESS)
                this->callback->moveDirection(0, IProjectileCallback::DROITE, deltaTime);
            if (glfwGetKey(global_window, GLFW_KEY_W) == GLFW_PRESS)
                this->callback->moveDirection(0, IProjectileCallback::DEVANT, deltaTime);
            if (glfwGetKey(global_window, GLFW_KEY_S) == GLFW_PRESS)
                this->callback->moveDirection(0, IProjectileCallback::DERRIERE, deltaTime);
            if (glfwGetKey(global_window, GLFW_KEY_SPACE) == GLFW_PRESS)
                this->callback->moveDirection(0, IProjectileCallback::HAUT, deltaTime);
            if (glfwGetKey(global_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                this->callback->moveDirection(0, IProjectileCallback::BAS, deltaTime);
         
            if (glfwGetKey(global_window, GLFW_KEY_Z) == GLFW_PRESS && !wireframeModePress)
            {
                global_wireframe = !global_wireframe;
                wireframeModePress = true;
            }
            else if (glfwGetKey(global_window, GLFW_KEY_Z) == GLFW_RELEASE && wireframeModePress)
            {
                wireframeModePress = false;
            }

            if (glfwGetKey(global_window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                this->callback->moveDirection(1, IProjectileCallback::DEVANT, deltaTime);
            }
            if (glfwGetKey(global_window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                this->callback->moveDirection(1, IProjectileCallback::DERRIERE, deltaTime);
            }
            if (glfwGetKey(global_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            {
                this->callback->moveDirection(1, IProjectileCallback::DROITE, deltaTime);
            }
            if (glfwGetKey(global_window, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                this->callback->moveDirection(1, IProjectileCallback::GAUCHE, deltaTime);
            }

            if (glfwGetKey(global_window, GLFW_KEY_K) == GLFW_PRESS)
            {
                this->callback->actionFireBall();
            }
            
        }
    }

public:
    InputProjectile(IGlobalGameCallback* gameCallback, IGlobalSceneCallback* sceneCallback, IProjectileCallback *callback) : Input(gameCallback, sceneCallback)
    {
        this->callback = callback;
    }
};

#endif