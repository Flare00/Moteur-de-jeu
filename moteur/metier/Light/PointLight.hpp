#ifndef __POINT_LIGHT_HPP__
#define __POINT_LIGHT_HPP__
#include <glm/glm.hpp>
#include <Light/ILight.hpp>
using namespace glm;

class PointLight : public ILight {
protected:
public:

	/// <summary>
	/// shadowResolution de 1 à 12, (calcul 2^n, si shadowResolution = 4 alors shadow map de taille 16, si shadowResolution = 12 alors shadow map de taille 4096)
	/// </summary>
	PointLight(glm::vec3 position, vec3 color = vec3(1.0f), float intensity = 1.0f) : ILight(Type::POINT, position, color, intensity) {
	}

	virtual void compute(GameObject* scene) {
	}
};

#endif
