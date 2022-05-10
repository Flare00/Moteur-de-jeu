#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <Callback/IGlobalGameCallback.hpp>
#include "Scene.hpp"
#include "SceneSpace.hpp"
#include "SceneCollision.hpp"
#include "SceneProjectile.hpp"
#include "SceneLight.hpp"
class Game : IGlobalGameCallback {
private :
	size_t activeScene = -1;
	size_t nbScene = 0;
	std::vector<Scene*> scenes;

public :
	Game() {
		//Add a Scene
		this->scenes.push_back(new SceneLight(this));
		this->scenes.push_back(new SceneProjectile(this));
		
		//Add here more Scene 

		//Scene Selector
		this->activeScene = 0;
		//number of scene
		nbScene = this->scenes.size();
		//Preload all scene
		for (int i = 0; i < nbScene; i++) {
			this->scenes[i]->Init();
		}
	}

	void Loop(float deltaTime) {

		if (this->activeScene >= 0 && this->activeScene < this->nbScene) {
			this->scenes[activeScene]->Draw(deltaTime);
		}
	}
	virtual void askSceneChange(int numero) {
		if (numero >= 0 && numero < this->nbScene) {
			this->activeScene = numero;
		}
	}

	virtual void askNextScene(bool next) {
		if (this->scenes.size() > 1) {
			if (next && this->activeScene + 1 >= this->scenes.size()) {
				this->activeScene = 0;
			}
			else if (!next && this->activeScene - 1 < 0) {
				this->activeScene = this->scenes.size() - 1;
			}
			else {
				this->activeScene = this->activeScene + (next ? 1 : -1);
			}
		}
	}
};


#endif