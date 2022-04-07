#ifndef __GRAVITY_HPP__
#define __GRAVITY_HPP__
#include <GameObject/GameObject.hpp>
#include <Component.hpp>
class Gravity : public Component {
private:
	const float constantGravity = 9.80;

	glm::vec3 vitesse;
	float masse = 1.0f; // kg
	
	GameObject* parent;

public:

	Gravity(GameObject* parent) : Component(Component::Type::GRAVITY) {
		this->parent = parent;
	}
	
	
	// Get set Mass

	void addAcceleration(glm::vec3 a)
	{
		this->vitesse += m*a;
	}

	void apply(float delta) {
		if (!collision) {
			float vitesse = delta * -constantGravity *0.5;
			parent->getTransform()->translate(glm::vec3(0, vitesse, 0));
		}
	}
};

#endif // !__GRAVITY_HPP__
