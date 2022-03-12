#ifndef __Scene_H__
#define __Scene_H__

#include <vector>
#include <string>

#include "../World/Camera.hpp"
#include "../World/Input.hpp"

#include "../GameObject/GameObject.hpp"
#include "../GameObject/Modele.hpp"
#include "../Transformation.hpp"
#include "../Texture.hpp"
#include "../World/Camera.hpp"
#include "../World/Input.hpp"

class Scene {
protected:
	std::string id;

	int activeCamera = -1;
	std::vector<Camera*> cameras;

	Input* input;
	GameObject* scene;

public:
	Scene(std::string id = "Scene") {
		this->id = id;
		this->scene = new GameObject(id);
	}

	~Scene() {
		free(this->input);
		for (int i = 0, max = this->cameras.size(); i < max; i++) {
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

	virtual void Init() {
	}
	virtual void Draw(float deltaTime) {
	}

	virtual void ChangeActiveCamera(int id) {
		this->activeCamera = id;
	}
};

#endif