#ifndef __Scene_H__
#define __Scene_H__

#include <vector>
#include <string>

#include <World/Camera.hpp>
#include <World/Input.hpp>

#include <GameObject/GameObject.hpp>
#include <Transformation/Transformation.hpp>
#include <Transformation/BulletTransformation.hpp>
#include <Texture.hpp>
#include <World/Camera.hpp>
#include <World/Input.hpp>
#include <Callback/IGlobalSceneCallback.hpp>

class Scene : public IGlobalSceneCallback {
protected:
	IGlobalGameCallback* globalGameCallback;
	std::string id;

	int activeCamera = -1;
	std::vector<Camera*> cameras;

	Input* input;
	GameObject* scene;

public:
	Scene(IGlobalGameCallback* globalGameCallback = NULL, std::string id = "Scene") {
		this->globalGameCallback;
		this->id = id;
		this->scene = new GameObject(id);
	}

	~Scene() {
		free(this->input);
		for (size_t i = 0, max = this->cameras.size(); i < max; i++) {
			free(this->cameras[i]);
		}
		free(this->scene);
	}

	void AddCamera(Camera* camera) {
		this->cameras.push_back(camera);
	}

	void SetInput(Input* input) {
		this->input = input;
	}

	void SetSceneGameObject(GameObject* scene) {
		this->scene = scene;
	}

	virtual void Init() = 0;
	virtual void Draw(float deltaTime) = 0;
	virtual void askCameraChange(int numero) {
		if (numero >= 0 && numero < this->cameras.size()) {
			this->activeCamera = numero;
		}
	}
	virtual void askNextCamera(bool next) {
		if (this->cameras.size() > 1) {
			if (next && this->activeCamera + 1 >= this->cameras.size()) {
				this->activeCamera = 0;
			}
			else if (!next && this->activeCamera - 1 < 0) {
				this->activeCamera = this->cameras.size() - 1;
			}
			else {
				this->activeCamera = this->activeCamera + (next ? 1 : -1);
			}
		}
	}

};

#endif