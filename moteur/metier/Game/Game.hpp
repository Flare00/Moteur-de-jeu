#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "Scene.hpp"
#include "SceneSpace.hpp"

#include "SceneCollision.hpp"

class Game {
private :
	int activeScene = -1;
	int nbScene = 0;
	std::vector<Scene*> scenes;
public :
	Game() {
		Scene* scene = new SceneCollision();
		scene->Init();
		this->scenes.push_back(scene);
		this->activeScene = 0;
		nbScene++;
	}

	void Loop(float deltaTime) {
		if (this->activeScene >= 0 && this->activeScene < this->nbScene) {
			this->scenes[activeScene]->Draw(deltaTime);
		}
	}
};


#endif