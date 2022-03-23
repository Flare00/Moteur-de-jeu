#ifndef __GRAVITY_HPP__
#define __GRAVITY_HPP__
#include <GameObject/GameObject.hpp>
#include <GameObject/GameObject.hpp>
#include <Component.hpp>
class Gravity : public Component {
private:
	const float constantGravity = 9.80;
	bool collision = false;
	GameObject* parent;
public:

	Gravity(GameObject* parent) : Component(Type::Gravity) {
		this->parent = parent;
	}
	void setCollision(bool collision) {
		collision = collision;
	}
	void apply(float delta) {
		if (!collision) {
			float vitesse = delta * -constantGravity * 0.1;
			parent->getTransform()->translate(glm::vec3(0, vitesse, 0));
		}
	}
};

#endif // !__GRAVITY_HPP__
