#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <Callback/IGlobalGameCallback.hpp>
#include "Scene.hpp"
#include "SceneProjectile.hpp"
#include "SceneLight.hpp"
class Game : IGlobalGameCallback
{
private:
	size_t activeScene = -1;
	size_t nbScene = 0;
	std::vector<Scene *> scenes;

	float waintingBeforeNextScene = 0.5f;
	float waintingBeforeNextSceneValue = 0;

public:
	Game()
	{
		// Add a Scene
		this->scenes.push_back(new SceneLight(this));
		this->scenes.push_back(new SceneProjectile(this));

		// Scene Selector
		this->activeScene = 1;
		// number of scene
		nbScene = this->scenes.size();
		this->scenes[activeScene]->Load();
	}

	void Loop(float deltaTime)
	{
		waintingBeforeNextSceneValue -= deltaTime;
		if (this->activeScene >= 0 && this->activeScene < this->nbScene)
		{
			if (this->scenes[activeScene]->isActive())
			{
				this->scenes[activeScene]->Draw(deltaTime);
			}
		}
	}
	virtual void askSceneChange(int numero)
	{
		if (numero >= 0 && numero < this->nbScene)
		{
			this->scenes[this->activeScene]->UnLoad();
			this->activeScene = numero;
			this->scenes[this->activeScene]->Load();
		}
	}

	virtual void askNextScene(bool next)
	{
		if (waintingBeforeNextSceneValue <= 0)
		{
			waintingBeforeNextSceneValue = waintingBeforeNextScene;
			if (this->scenes.size() > 1)
			{
				this->scenes[this->activeScene]->UnLoad();
				if (next && this->activeScene + 1 >= this->scenes.size())
				{
					this->activeScene = 0;
				}
				else if (!next && this->activeScene - 1 < 0)
				{
					this->activeScene = this->scenes.size() - 1;
				}
				else
				{
					this->activeScene = this->activeScene + (next ? 1 : -1);
				}

				this->scenes[this->activeScene]->Load();
			}
		}
	}
};

#endif