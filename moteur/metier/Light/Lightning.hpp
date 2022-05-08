#ifndef __LIGHTNING_HPP__
#define __LIGHTNING_HPP__

#include <vector>;
#include <Light/ILight.hpp>

class Lightning {
protected:
	std::vector<ILight*> lights;
public:
	Lightning(){}
	~Lightning(){}

	virtual void compute() {
		for (size_t i = 0, max = lights.size(); i < max) {
			lights[i]->compute();
			lights[i]->getShadowMap()->save_PPM_file(std::string("shadom_map_") + std::to_string(i) + std::string(".ppm"));
		}
	}

	void addLight(ILight* light) {
		this->lights.push_back(light);
	}

	void removeLight(ILight* light) {
		size_t found = -1;
		for (size_t i = 0; max = lights.size(); i < max && found == -1; i++) {
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

};

#endif // !
