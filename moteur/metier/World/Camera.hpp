#ifndef __H_CAMERA__
#define __H_CAMERA__

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;

const float MOVE_SPEED = 10.0f;
const float ROTATE_SPEED = 50.0f;

using namespace glm;

enum CameraAxe {
	X,
	Y,
	Z
};

class Camera {
public:

	//transformation
	glm::mat4 transformation;

	//position
	vec3 position;

	//referentiel
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 worldUp;

	//Angle d'Euler
	float yaw;
	float pitch;
	float roll;

	//options
	float moveSpeed;
	float rotateSpeed;

	//Orbital
	vec3 cibleOrbital = vec3(0.0f);
	bool modeOrbital = false;

public:
	Camera(vec3 pos = vec3(0.0f, 0.0f, 3.0f), float yaw = YAW, float pitch = PITCH, vec3 up = vec3(0.0f, 1.0f, 0.0f)) {
		this->transformation = glm::mat4(1.0f);
		this->front = vec3(0.0f, 0.0f, -1.0f);
		this->moveSpeed = MOVE_SPEED;
		this->rotateSpeed = ROTATE_SPEED;
		this->roll = ROLL;
		this->worldUp = up;
		this->position = pos;
		this->pitch = pitch;
		this->yaw = yaw;
		this->up = up;
		updateVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	{
		this->transformation = glm::mat4(1.0f);
		this->front = vec3(0.0f, 0.0f, -1.0f);
		this->moveSpeed = MOVE_SPEED;
		this->rotateSpeed = ROTATE_SPEED;

		this->roll = ROLL;
		this->position = vec3(posX, posY, posZ);
		this->worldUp = vec3(upX, upY, upZ);
		this->up = vec3(upX, upY, upZ);

		this->yaw = yaw;
		this->pitch = pitch;
		updateVectors();

	}

	mat4 getProjection() {
		return glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);
	}

	mat4 getViewMatrix() {
		if (modeOrbital) {
			//return lookAt(this->position, this->cibleOrbital, this->up);
		}
		return lookAt(this->position, this->position + this->front, this->up);
	}

	void move(CameraAxe axe, bool sensPositif, float deltaTime) {
		if (!modeOrbital) {
			float vitesse = this->moveSpeed * deltaTime * (sensPositif ? 1.0f : -1.0f);
			switch (axe) {
			case CameraAxe::X:
				this->position += this->right * vitesse;
				break;
			case CameraAxe::Y:
				this->position += this->up * vitesse;
				break;
			case CameraAxe::Z:
				this->position += this->front * vitesse;
				break;
			}
		}
		else {
			rotate(axe, sensPositif, deltaTime);
		}

	}


	void rotate(CameraAxe axe, bool sensPositif, float deltaTime) {
		float vitesse = this->rotateSpeed * deltaTime * (sensPositif ? 1.0f : -1.0f);
		switch (axe) {
		case CameraAxe::X:
			this->pitch += vitesse;
			break;
		case CameraAxe::Y:
			this->yaw += vitesse;
			break;
		case CameraAxe::Z:
			this->roll += vitesse;
			break;
		}
		updateVectors();
	}

	void rotate(CameraAxe axe, float value) {
		float vitesse = this->rotateSpeed * value;
		switch (axe) {
		case CameraAxe::X:
			this->pitch += vitesse;
			break;
		case CameraAxe::Y:
			this->yaw += vitesse;
			break;
		case CameraAxe::Z:
			this->roll += vitesse;
			break;
		}
		updateVectors();
	}

	void setOrbitalCible(vec3 pos) {
		this->cibleOrbital = pos;
	}

	void switchOrbitalMode() {
		this->modeOrbital = !this->modeOrbital;
	}

	glm::mat4 getTransformationMatrix() {
		return this->transformation;
	}



private:
	void updateVectors() {
		if (!modeOrbital) {
			glm::vec3 front;
			front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			front.y = sin(glm::radians(this->pitch));
			front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

			this->front = glm::normalize(front);
			this->right = normalize(cross(this->front, this->worldUp));
			this->up = normalize(cross(this->right, this->front));
		}
		else {

		}
	}

};

#endif