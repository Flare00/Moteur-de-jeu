#ifndef __POINT_LIGHT_HPP__
#define __POINT_LIGHT_HPP__
#include <glm/glm.hpp>
#include <Light/ILight.hpp>
using namespace glm;

class PointLight : public ILight
{
protected:
public:
	PointLight(glm::vec3 position, vec3 color = vec3(1.0f), float intensity = 1.0f) : ILight(Type::POINT, position, color, intensity)
	{
	}

	virtual void compute(GameObject *scene)
	{
	}
};

#endif
