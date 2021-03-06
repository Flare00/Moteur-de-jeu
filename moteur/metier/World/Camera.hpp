#ifndef __H_CAMERA__
#define __H_CAMERA__

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <World/Frustum.hpp>
#include <Physique/BulletRigidbody.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <World/CameraData.hpp>
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;

const float MOVE_SPEED = 10.0f;
const float ROTATE_SPEED = 50.0f;

using namespace glm;

enum CameraAxe
{
	X,
	Y,
	Z
};

class Camera
{
public:
	CameraData data;
	// transformation
	glm::mat4 transformation;

	// referentiel
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 worldUp;

	// Angle d'Euler
	float yaw;
	float pitch;
	float roll;

	// options
	float moveSpeed;
	float rotateSpeed;

	// Orbital
	vec3 cibleOrbital = vec3(0.0f);
	bool modeOrbital = false;

	float distanceMax = 10000.0f;
	float distanceMin = 0.01f;

	float fov = 45.0f;
	float aspect = 4.0f / 3.0f;

	bool dirty = true;

public:
	Camera(vec3 pos = vec3(0.0f, 0.0f, 3.0f), float yaw = YAW, float pitch = PITCH, vec3 up = vec3(0.0f, 1.0f, 0.0f), btGhostObject *bulletFrustum = NULL) : data(CameraData::CAMERA, new Frustum(), pos)
	{
		this->transformation = glm::mat4(1.0f);
		this->front = vec3(0.0f, 0.0f, -1.0f);
		this->moveSpeed = MOVE_SPEED;
		this->rotateSpeed = ROTATE_SPEED;
		this->roll = ROLL;
		this->worldUp = up;
		this->pitch = pitch;
		this->yaw = yaw;
		this->up = up;
		updateVectors();

		data.getFrustum()->init(glm::mat4(1.0f), glm::mat4(1.0f), fov, aspect, distanceMin, distanceMax);
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, btGhostObject *bulletFrustum = NULL) : data(CameraData::CAMERA, new Frustum(), glm::vec3(posX, posY, posZ))
	{
		this->transformation = glm::mat4(1.0f);
		this->front = vec3(0.0f, 0.0f, -1.0f);
		this->moveSpeed = MOVE_SPEED;
		this->rotateSpeed = ROTATE_SPEED;

		this->roll = ROLL;
		this->worldUp = vec3(upX, upY, upZ);
		this->up = vec3(upX, upY, upZ);

		this->yaw = yaw;
		this->pitch = pitch;
		updateVectors();

		data.getFrustum()->init(this->getProjection(), this->getViewMatrix(), this->fov, this->aspect, this->distanceMin, this->distanceMax);
	}

	void checkUpdate()
	{
		if (dirty)
		{
			this->updateData();
		}
	}

	mat4 getProjection()
	{
		if (dirty)
		{
			updateData();
		}
		return this->data.getProjection();
	}

	mat4 getViewMatrix()
	{
		if (dirty)
		{
			updateData();
		}

		return this->data.getView();
	}

	void move(CameraAxe axe, bool sensPositif, float deltaTime)
	{
		if (!modeOrbital)
		{
			float vitesse = this->moveSpeed * deltaTime * (sensPositif ? 1.0f : -1.0f);
			switch (axe)
			{
			case CameraAxe::X:
				data.setPosition(data.getPosition() + this->right * vitesse);
				break;
			case CameraAxe::Y:
				data.setPosition(data.getPosition() + this->up * vitesse);
				break;
			case CameraAxe::Z:
				data.setPosition(data.getPosition() + this->front * vitesse);
				break;
			}
		}
		else
		{
			rotate(axe, sensPositif, deltaTime);
		}
		dirty = true;
	}

	void rotate(CameraAxe axe, bool sensPositif, float deltaTime)
	{
		float vitesse = this->rotateSpeed * deltaTime * (sensPositif ? 1.0f : -1.0f);
		switch (axe)
		{
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

	void rotate(CameraAxe axe, float value)
	{
		float vitesse = this->rotateSpeed * value;
		switch (axe)
		{
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

	void setOrbitalCible(vec3 pos)
	{
		this->cibleOrbital = pos;
	}

	void switchOrbitalMode()
	{
		this->modeOrbital = !this->modeOrbital;
	}

	glm::mat4 getTransformationMatrix()
	{
		if (dirty)
		{
			updateData();
		}
		return this->transformation;
	}

	glm::vec3 getPosition()
	{
		if (dirty)
		{
			updateData();
		}
		return data.getPosition();
	}

	float distanceFromCamera(glm::vec3 point)
	{
		if (dirty)
		{
			updateData();
		}
		return glm::distance(data.getPosition(), point);
	}

	CameraData *getData()
	{
		if (dirty)
		{
			updateData();
			dirty = false;
		}
		return &this->data;
	}

	glm::vec3 getFront()
	{
		return this->front;
	}

private:
	void updateVectors()
	{
		if (!modeOrbital)
		{
			glm::vec3 front;
			front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			front.y = sin(glm::radians(this->pitch));
			front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

			this->front = glm::normalize(front);
			this->right = normalize(cross(this->front, this->worldUp));
			this->up = normalize(cross(this->right, this->front));
		}
		else
		{
		}
		dirty = true;
	}

	void updateData()
	{
		this->data.setValues(data.getPosition(), glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, this->distanceMin, this->distanceMax), lookAt(data.getPosition(), data.getPosition() + this->front, this->up));
		this->data.getFrustum()->update(data.getProjection(), data.getView(), this->front, this->up, this->right, data.getPosition());
		this->dirty = false;
	}
};

#endif