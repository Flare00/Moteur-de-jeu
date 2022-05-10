#ifndef __LIGHTNING_HPP__
#define __LIGHTNING_HPP__

#include <vector>
#include <Light/ILight.hpp>
#include <GameObject/GameObject.hpp>
#include <Global.hpp>

class Lightning {
protected:
	std::vector<ILight*> lights;
public:
	Lightning() {}
	~Lightning() {}

	void compute(GameObject* scene) {
		for (size_t i = 0, max = lights.size(); i < max; i++) {
			lights[i]->compute(scene);
			ShadowMap::bindScreen();
			glViewport(0, 0, screen_width, screen_height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
 // Erase the color and z buffers.
	}

	void addLight(ILight* light) {
		this->lights.push_back(light);
	}

	void removeLight(ILight* light) {
		size_t found = -1;
		for (size_t i = 0, max = lights.size(); i < max && found == -1; i++) {
			if (lights[i] == light) {
				found = i;
			}
		}
		if (found != -1) {
			lights.erase(lights.begin() + found);
		}
	}

	void clearLights() {
		this->lights.clear();
	}

	std::vector<ILight*> getLights() {
		return this->lights;
	}

};

#endif // !
