#ifndef __GRAVITY_HPP__
#define __GRAVITY_HPP__

#include "../Component.hpp"
class Gravity : public Component {
private:
	const float constantGravity = 9.80;
	bool collision = false;
public:

	void setCollision(bool collision) {
		collision = collision;
	}
	virtual void apply(GameObject* parent) {
		if (!collision) {
		}
	}

};

#endif // !__GRAVITY_HPP__
