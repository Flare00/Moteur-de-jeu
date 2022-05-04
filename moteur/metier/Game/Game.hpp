#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "Scene.hpp"
#include "SceneSpace.hpp"

#include "SceneCollision.hpp"

class Game {
private :
	size_t activeScene = -1;
	size_t nbScene = 0;
	std::vector<Scene*> scenes;
public :
	Game() {
		//Add a Scene
		Scene* scene = new SceneCollision();
		scene->Init();
		this->scenes.push_back(scene);
		
		//Add here more Scene 

		//Scene Selector
		this->activeScene = 0;
		//number of scene
		nbScene = this->scenes.size();
	}

	void Loop(float deltaTime) {
		if (this->activeScene >= 0 && this->activeScene < this->nbScene) {
			this->scenes[activeScene]->Draw(deltaTime);
		}
	}
};


#endif